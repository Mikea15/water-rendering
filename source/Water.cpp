#include "include/main.h"

const float g_NormalMapScale = 0.25f;

bool AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;

	float currentTime = GetTickCount() * 0.001f;
	float deltaTime = currentTime - lastTime;
	float desiredFPS = 1.0f / desiredFrameRate;

	elapsedTime += deltaTime;
	lastTime = currentTime;

	if (elapsedTime > desiredFPS)
	{
		g_DeltaTime = desiredFPS;
		elapsedTime -= desiredFPS;
		return true;
	}

	return false;
}

void LoadAnimTextures(UINT textureArray[], LPSTR fileName, int startIndex, int bitmapCount)
{
	char szBuffer[255];
	int i = 0;

	if (!fileName)
		return;

	for (i = 0; i < bitmapCount; i++)
	{
		sprintf_s(szBuffer, "%s%d%d.bmp", fileName, i / 10, i % 10);
		CreateTexture(textureArray[startIndex + i], szBuffer);
	}
}

void CreateRenderTexture(UINT textureArray[], int size, int channels, int type, int textureID)
{
	unsigned int *pTexture = NULL;

	pTexture = new unsigned int[size * size * channels];
	memset(pTexture, 0, size * size * channels * sizeof(unsigned int));

	glGenTextures(1, &textureArray[textureID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, pTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	delete[] pTexture;}

void RenderCaustics(float waterHeight, float causticScale)
{
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);

	static int startIndex = 0;

	glBindTexture(GL_TEXTURE_2D, g_Texture[startIndex + WATER_START_ID]);

	static int frameCount = 0;

	if (frameCount == 5)
	{
		startIndex = ((startIndex + 1) % NUM_WATER_TEX);
		frameCount = 0;
	}

	frameCount++;

	glMatrixMode(GL_TEXTURE);

	glLoadIdentity();
	glScalef(causticScale, causticScale, 1);

	glMatrixMode(GL_MODELVIEW);

	double plane[4] = { 0.0, -1.0, 0.0, waterHeight };

	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);

	RenderHeightMap(g_HeightMap);

	glDisable(GL_CLIP_PLANE0);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
}

void CreateReflectionTexture(float waterHeight, int textureSize)
{
	glViewport(0, 0, textureSize, textureSize);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	g_Camera.Look();

	glPushMatrix();

	if (g_Camera.Position().y > waterHeight)
	{
		glTranslatef(0.0f, waterHeight*2.0f, 0.0f);
		glScalef(1.0, -1.0, 1.0);

		glCullFace(GL_FRONT);

		double plane[4] = { 0.0, 1.0, 0.0, -waterHeight };
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane);

		RenderWorld(false);

		glDisable(GL_CLIP_PLANE0);

		glCullFace(GL_BACK);
	}
	else
	{
		double plane[4] = { 0.0, 1.0, 0.0, waterHeight };
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane);
		RenderWorld(true);
		glDisable(GL_CLIP_PLANE0);
	}

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, g_Texture[REFLECTION_ID]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, textureSize, textureSize);
}

void CreateRefractionDepthTexture(float waterHeight, int textureSize)
{
	glViewport(0, 0, textureSize, textureSize);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	g_Camera.Look();

	glPushMatrix();

	if (g_Camera.Position().y > waterHeight)
	{
		double plane[4] = { 0.0, -1.0, 0.0, waterHeight };
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane);
		RenderWorld(true);
		glDisable(GL_CLIP_PLANE0);
	}
	else
	{
		glCullFace(GL_FRONT);
		double plane[4] = { 0.0, 1.0, 0.0, -waterHeight };
		glEnable(GL_CLIP_PLANE0);
		glClipPlane(GL_CLIP_PLANE0, plane);
		RenderWorld(true);
		glDisable(GL_CLIP_PLANE0);
		glCullFace(GL_BACK);
	}

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, g_Texture[REFRACTION_ID]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, textureSize, textureSize);

	glBindTexture(GL_TEXTURE_2D, g_Texture[DEPTH_ID]);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, textureSize, textureSize, 0);
}

void RenderWater(float waterHeight)
{
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_Texture[REFLECTION_ID]);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_Texture[REFRACTION_ID]);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_Texture[NORMAL_ID]);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_Texture[DUDVMAP_ID]);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_Texture[DEPTH_ID]);

	GLint uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "reflection");
	glUniform1iARB(uniform, 0);

	uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "refraction");
	glUniform1iARB(uniform, 1);

	uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "normalMap");
	glUniform1iARB(uniform, 2);

	uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "dudvMap");
	glUniform1iARB(uniform, 3);

	uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "depthMap");
	glUniform1iARB(uniform, 4);

	uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "waterColor");
	glUniform4fARB(uniform, 0.1f, 0.2f, 0.4f, 1.0f);

	glm::vec3 lightPos(100.0f, 150.0f, 100.0f);
	glm::vec3 position = g_Camera.Position();

	uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "lightPos");
	glUniform4fARB(uniform, lightPos.x, lightPos.y, lightPos.z, 1.0f);

	uniform = glGetUniformLocationARB(g_Shader.GetProgram(), "cameraPos");
	glUniform4fARB(uniform, position.x, position.y, position.z, 1.0f);

	static float move = 0.0f;

	float move2 = move * g_NormalMapScale;

	float refrUV = g_WaterUV;

	float normalUV = g_WaterUV * g_NormalMapScale;

	move += g_WaterFlow;

	glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, g_WaterUV);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, refrUV - move);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, normalUV + move2);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0, 0);
		glVertex3f(0.0f, waterHeight, 0.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f - move);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 0.0f + move2);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0, 0);
		glVertex3f(0.0f, waterHeight, 1000.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, g_WaterUV, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, refrUV, 0.0f - move);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, normalUV, 0.0f + move2);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0, 0);
		glVertex3f(1000.0f, waterHeight, 1000.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, g_WaterUV, g_WaterUV);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, refrUV, refrUV - move);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, normalUV, normalUV + move2);
		glMultiTexCoord2fARB(GL_TEXTURE3_ARB, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE4_ARB, 0, 0);
		glVertex3f(1000.0f, waterHeight, 0.0f);
	glEnd();

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_2D);
}