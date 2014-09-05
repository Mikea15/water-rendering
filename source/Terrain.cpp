#include "include/main.h"

extern float g_FogDepth;
extern bool g_Detail;
extern int g_DetailScale;

int Height(BYTE *heightMap, int X, int Y)
{
	int x = X % MAP_SIZE;
	int y = Y % MAP_SIZE;

	if (!heightMap) 
		return 0;

	return heightMap[x + (y * MAP_SIZE)];
}

void SetTextureCoord(float x, float z, float height)
{
	float u = (float)x / (float)MAP_SIZE;
	float v = -(float)z / (float)MAP_SIZE;

	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);

	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);

	glMultiTexCoord2fARB(GL_TEXTURE2_ARB, u, v);
}

void SetFogCoord(float depth, float height)
{
	float fogY = 0;

	if (height > depth)
		fogY = 0;
	else
		fogY = -(height - depth);

	glFogCoordfEXT(fogY);
}

void RenderHeightMap(BYTE heightMap[])
{
	int X = 0, Y = 0;
	int x, y, z;
	bool switchSides = false;

	if (!heightMap)
		return;

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_Texture[TERRAIN_ID]);

	if (g_Detail)
	{
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);

		glBindTexture(GL_TEXTURE_2D, g_Texture[DETAIL_ID]);

		glMatrixMode(GL_TEXTURE);

		glLoadIdentity();
		glScalef((float)g_DetailScale, (float)g_DetailScale, 1);

		glMatrixMode(GL_MODELVIEW);
	}

	glBegin(GL_TRIANGLE_STRIP);

	for (X = 0; X <= MAP_SIZE; X += STEP_SIZE)
	{
		if (switchSides)
		{
			for (Y = MAP_SIZE; Y >= 0; Y -= STEP_SIZE)
			{
				x = X;
				y = Height(heightMap, X, Y);
				z = Y;

				SetFogCoord(g_FogDepth, (float)y);

				SetTextureCoord((float)x, (float)z, (float)y);
				glVertex3i(x, y, z);

				x = X + STEP_SIZE;
				y = Height(heightMap, X + STEP_SIZE, Y);
				z = Y;

				SetFogCoord(g_FogDepth, (float)y);

				SetTextureCoord((float)x, (float)z, (float)y);
				glVertex3i(x, y, z);
			}
		}
		else
		{
			for (Y = 0; Y <= MAP_SIZE; Y += STEP_SIZE)
			{
				x = X + STEP_SIZE;
				y = Height(heightMap, X + STEP_SIZE, Y);
				z = Y;

				SetFogCoord(g_FogDepth, (float)y);

				SetTextureCoord((float)x, (float)z, (float)y);
				glVertex3i(x, y, z);

				x = X;
				y = Height(heightMap, X, Y);
				z = Y;

				SetFogCoord(g_FogDepth, (float)y);

				SetTextureCoord((float)x, (float)z, (float)y);
				glVertex3i(x, y, z);
			}
		}

		switchSides = !switchSides;
	}

	glEnd();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
}

void LoadRawFile(LPSTR filename, int size, BYTE *heightMap)
{
	FILE *infile;

	fopen_s(&infile, filename, "rb");

	if (infile == NULL)
	{
		MessageBox(NULL, "Can't find the height map!", "Error", MB_OK);
		return;
	}

	fread(heightMap, 1, size, infile);

	int result = ferror(infile);

	if (result)
	{
		MessageBox(NULL, "Can't get data!", "Error", MB_OK);
	}

	fclose(infile);
}
