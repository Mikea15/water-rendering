#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")

#include "include/main.h"

#pragma region Externs

UINT g_Texture[MAX_TEXTURES] = { 0 };

HDC g_DeviceHandle;
HGLRC g_GLContextHandle;
HINSTANCE g_WindowInstanceHandle;
HWND g_WindowHandle;
RECT g_Rect;
bool g_FullScreen = true;

float g_DeltaTime = 0.0f;
float g_FogDepth = 30.0f;
float g_WaterFlow = 0.0015f;
float g_WaterHeight = 30.0f;
float g_WaterUV = 35.0f;

GCamera g_Camera;
Shader g_Shader;

BYTE g_HeightMap[MAP_SIZE * MAP_SIZE];
int g_TextureSize = 512;

bool g_Detail = true;
bool g_RenderMode = true;
int g_DetailScale = 16;

char normalMap[260];
char dudvMap[260];

#pragma endregion

WPARAM MainLoop()
{
	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (AnimateNextFrame(60))
			{
				g_Camera.Update();
				RenderScene();
			}
			else
			{
				Sleep(1);
			}
		}
	}

	DeInit();
	return(msg.wParam);
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT    ps;

	switch (message)
	{
	case WM_SIZE:
		if (!g_FullScreen)
		{
			SizeOpenGLScreen(LOWORD(lParam), HIWORD(lParam));
			GetClientRect(hwnd, &g_Rect);
		}
		break;

	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		g_Detail = !g_Detail;
		break;

	case WM_RBUTTONDOWN:
		g_RenderMode = !g_RenderMode;

		if (g_RenderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		case VK_SPACE:
			g_DetailScale = (g_DetailScale * 2) % 128;

			if (g_DetailScale == 0)
				g_DetailScale = 1;
			break;

		case VK_ADD:
			g_FogDepth += 1;

			if (g_FogDepth > 200)
				g_FogDepth = 200;
			break;

		case VK_SUBTRACT:
			g_FogDepth -= 1;

			if (g_FogDepth < 0)
				g_FogDepth = 0;
			break;

		case VK_F1:
			g_WaterUV += 1.0f;
			break;

		case VK_F2:
			g_WaterUV -= 1.0f;
			if (g_WaterUV < 0.0f)
				g_WaterUV = 0.0f;
			break;

		case VK_F3:
			g_WaterFlow += 0.00003f;
			break;

		case VK_F4:
			g_WaterFlow -= 0.00003f;

			if (g_WaterFlow < 0.0f)
				g_WaterFlow = 0.0f;
			break;
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void GetMaps()
{
	OPENFILENAME ofn;
	char file[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = file;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(file);
	ofn.lpstrFilter = "All\0*.*\0BMP\0*.BMP\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// We save the current directory since the code below can change it
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	if (MessageBox(NULL, "Choose normal map file or use default?", "Options", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		if (GetOpenFileName(&ofn) == TRUE)
		{
			memcpy_s(normalMap, 260, ofn.lpstrFile, 260);
		}
		else
		{
			char normal[] = "Textures\\normalmap.bmp";
			memcpy_s(normalMap, sizeof(normal), normal, sizeof(normal));
		}
	}
	else
	{
		char normal[] = "Textures\\normalmap.bmp";
		memcpy_s(normalMap, sizeof(normal), normal, sizeof(normal));
	}

	if (MessageBox(NULL, "Choose dudv map file or use default?", "Options", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		if (GetOpenFileName(&ofn) == TRUE)
		{
			memcpy_s(dudvMap, 260, ofn.lpstrFile, 260);
		}
		else
		{
			char normal[] = "Textures\\dudvmap.bmp";
			memcpy_s(normalMap, sizeof(normal), normal, sizeof(normal));
		}
	}
	else
	{
		char dudv[] = "Textures\\dudvmap.bmp";
		memcpy_s(dudvMap, sizeof(dudv), dudv, sizeof(dudv));
	}

	// Restore the working directory
	SetCurrentDirectory(NPath);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{
	HWND windowHandle;

	GetMaps();

	if (MessageBox(NULL, "Click Yes to go to full screen (Recommended)", "Options", MB_YESNO | MB_ICONQUESTION) == IDNO)
		g_FullScreen = false;

	windowHandle = MakeWindow("Water Rendering", SCREEN_WIDTH, SCREEN_HEIGHT, 0, g_FullScreen, hInstance);

	if (windowHandle == NULL)
		return TRUE;

	Init(windowHandle);
	return (int)MainLoop();
}