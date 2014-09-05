#include "include/main.h"

bool CreateTexture(GLuint &textureID, LPSTR filename)
{
	HBITMAP bmpHandle;
	BITMAP bitmap;

	glGenTextures(1, &textureID);
	bmpHandle = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (!bmpHandle)
		return FALSE;

	GetObject(bmpHandle, sizeof(bitmap), &bitmap);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.bmWidth, bitmap.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap.bmBits);
	DeleteObject(bmpHandle);

	return TRUE;
}

void Init(HWND handle)
{
	g_WindowHandle = handle;
	GetClientRect(g_WindowHandle, &g_Rect);
	InitializeOpenGL(g_Rect.right, g_Rect.bottom);

	float fogColor[4] = { 0.2f, 0.2f, 0.9f, 1.0f };

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_START, 0.0);
	glFogf(GL_FOG_END, 50.0);

	glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);

	LoadRawFile("maps\\Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	g_Camera.PositionCamera(
		glm::vec3(475, 52, 301),
		glm::vec3(474, 52, 300),
		glm::vec3(0, 1, 0));

	CreateRenderTexture(g_Texture, g_TextureSize, 3, GL_RGB, REFLECTION_ID);
	CreateRenderTexture(g_Texture, g_TextureSize, 3, GL_RGB, REFRACTION_ID);
	CreateRenderTexture(g_Texture, g_TextureSize, 1, GL_DEPTH_COMPONENT, DEPTH_ID);

	CreateTexture(g_Texture[NORMAL_ID], normalMap);
	CreateTexture(g_Texture[DUDVMAP_ID], dudvMap);
	CreateTexture(g_Texture[TERRAIN_ID], "Textures\\Terrain.bmp");
	CreateTexture(g_Texture[DETAIL_ID], "Textures\\Detail.bmp");
	CreateTexture(g_Texture[BACK_ID], "Textures\\Back.bmp");
	CreateTexture(g_Texture[FRONT_ID], "Textures\\Front.bmp");
	CreateTexture(g_Texture[BOTTOM_ID], "Textures\\Bottom.bmp");
	CreateTexture(g_Texture[TOP_ID], "Textures\\Top.bmp");
	CreateTexture(g_Texture[LEFT_ID], "Textures\\Left.bmp");
	CreateTexture(g_Texture[RIGHT_ID], "Textures\\Right.bmp");

	LoadAnimTextures(g_Texture, "Textures\\caust", WATER_START_ID, NUM_WATER_TEX);

	InitGLSL();

	Shader::ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER_ARB, "shaders\\water.vert" },
			{ GL_FRAGMENT_SHADER_ARB, "shaders\\water.frag" },
			{ GL_NONE, NULL }
	};
	g_Shader.LoadShaders(shaders);

	g_Shader.TurnOff();
}

void InitializeOpenGL(int width, int height)
{
	g_DeviceHandle = GetDC(g_WindowHandle);

	if (!SetupPixelFormat(g_DeviceHandle))
		PostQuitMessage(0);

	g_GLContextHandle = wglCreateContext(g_DeviceHandle);
	wglMakeCurrent(g_DeviceHandle, g_GLContextHandle);

	SizeOpenGLScreen(width, height);
}

void DeInit()
{
	if (g_GLContextHandle)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(g_GLContextHandle);
	}

	if (g_DeviceHandle)
		ReleaseDC(g_WindowHandle, g_DeviceHandle);

	if (g_FullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	UnregisterClass("WaterRendering", g_WindowInstanceHandle);
	PostQuitMessage(0);
}
