#include "include/main.h"

const float g_CausticScale = 4.0f;

void RenderScene()
{
	CreateReflectionTexture(g_WaterHeight, g_TextureSize);
	CreateRefractionDepthTexture(g_WaterHeight, g_TextureSize);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glm::vec3 pos = g_Camera.Position();
	glm::vec3 newPos = pos;

	if (pos.y < Height(g_HeightMap, (int)pos.x, (int)pos.z) + 10)
	{
		newPos.y = (float)Height(g_HeightMap, (int)pos.x, (int)pos.z) + 10;

		// Get the difference of the y that the camera was pushed back up
		float temp = newPos.y - pos.y;

		//  Get the current view and increase it by the different the position was moved
		glm::vec3 view = g_Camera.View();
		view.y += temp;

		// Set the new camera position.
		g_Camera.PositionCamera(newPos, view, glm::vec3(0, 1, 0));
	}

	g_Camera.Look();

	RenderWorld(true);

	g_Shader.TurnOn();

	glDisable(GL_CULL_FACE);
	RenderWater(g_WaterHeight);
	glEnable(GL_CULL_FACE);

	g_Shader.TurnOff();
	SwapBuffers(g_DeviceHandle);
}

void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	glEnable(GL_TEXTURE_2D);

	// This centers the sky box around (x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	// Bind the BACK texture of the sky map and draw it
	glBindTexture(GL_TEXTURE_2D, g_Texture[BACK_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z);
	glEnd();

	// Bind the FRONT texture of the sky map and draw it
	glBindTexture(GL_TEXTURE_2D, g_Texture[FRONT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z + length);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z + length);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z + length);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z + length);
	glEnd();

	// Bind the BOTTOM texture of the sky map and draw it
	glBindTexture(GL_TEXTURE_2D, g_Texture[BOTTOM_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y, z + length);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y, z + length);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z);
	glEnd();

	// Bind the TOP texture of the sky map and draw it
	glBindTexture(GL_TEXTURE_2D, g_Texture[TOP_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y + height, z + length);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y + height, z + length);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z);
	glEnd();

	// Bind the LEFT texture of the sky map and draw it
	glBindTexture(GL_TEXTURE_2D, g_Texture[LEFT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z + length);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z + length);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z);
	glEnd();

	// Bind the RIGHT texture of the sky map and draw it
	glBindTexture(GL_TEXTURE_2D, g_Texture[RIGHT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z + length);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z + length);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z);
	glEnd();
}

void RenderWorld(bool renderCaustics)
{
	if (renderCaustics)
		RenderCaustics(g_WaterHeight, g_CausticScale);

	double TopTerrainPlane[4] = { 0.0, 1.0, 0.0, -g_WaterHeight };

	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, TopTerrainPlane);

	RenderHeightMap(g_HeightMap);

	glDisable(GL_CLIP_PLANE0);
	CreateSkyBox(500, 0, 500, 2000, 2000, 2000);
}