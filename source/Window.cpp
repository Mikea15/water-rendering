#include "include/main.h"

void ChangeToFullScreen()
{
	DEVMODE devSettings = { 0 };
	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devSettings))
	{
		MessageBox(NULL, "Could Not Enum Display Settings", "Error", MB_OK);
		return;
	}

	devSettings.dmPelsWidth = SCREEN_WIDTH;
	devSettings.dmPelsHeight = SCREEN_HEIGHT;
	devSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	int result = ChangeDisplaySettings(&devSettings, CDS_FULLSCREEN);

	if (result != DISP_CHANGE_SUCCESSFUL)
	{
		MessageBox(NULL, "Display Mode Not Compatible", "Error", MB_OK);
		PostQuitMessage(0);
	}
}

HWND MakeWindow(LPSTR windowName, int width, int height, DWORD style, bool fullScreen, HINSTANCE instance)
{
	HWND windowHandle;
	WNDCLASS windowClass;

	memset(&windowClass, 0, sizeof(WNDCLASS));
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WinProc;
	windowClass.hInstance = instance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszClassName = "GameTutorials";

	RegisterClass(&windowClass);

	if (fullScreen && !style)
	{
		style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ChangeToFullScreen();
		ShowCursor(FALSE);
	}
	else if (!style)
	{
		style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	g_WindowInstanceHandle = instance;

	RECT window;
	window.left = 0;
	window.right = width;
	window.top = 0;
	window.bottom = height;

	AdjustWindowRect(&window, style, false);

	windowHandle = CreateWindow("GameTutorials", windowName, style, 0, 0,
		window.right - window.left, window.bottom - window.top,
		NULL, NULL, instance, NULL);

	if (!windowHandle)
		return NULL;

	ShowWindow(windowHandle, SW_SHOWNORMAL);
	UpdateWindow(windowHandle);

	SetFocus(windowHandle);

	return windowHandle;
}

bool SetupPixelFormat(HDC handle)
{
	PIXELFORMATDESCRIPTOR pixel = { 0 };
	int pixelFormat;

	pixel.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel.nVersion = 1;

	pixel.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixel.dwLayerMask = PFD_MAIN_PLANE;
	pixel.iPixelType = PFD_TYPE_RGBA;
	pixel.cColorBits = SCREEN_DEPTH;
	pixel.cDepthBits = SCREEN_DEPTH;
	pixel.cAccumBits = 0;
	pixel.cStencilBits = 0;

	if ((pixelFormat = ChoosePixelFormat(handle, &pixel)) == FALSE)
	{
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(handle, pixelFormat, &pixel) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

void SizeOpenGLScreen(int width, int height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.9f, 4000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}