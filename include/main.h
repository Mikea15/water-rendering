#ifndef _MAIN_H
#define _MAIN_H

#include <GL/glew.h>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "GCamera.h"
#include "Init.h"
#include "Render.h"
#include "Shader.h"
#include "Terrain.h"
#include "Water.h"
#include "Window.h"

#pragma region Defines

// Maximum number of textures we can load
#define MAX_TEXTURES 1000

// The ids for the different maps
#define REFLECTION_ID	0
#define REFRACTION_ID	1
#define NORMAL_ID		2
#define DUDVMAP_ID		3
#define DEPTH_ID		4
#define TERRAIN_ID		5
#define DETAIL_ID		6

// The ids for the sides of the sky cube
#define BACK_ID		11
#define FRONT_ID	12
#define BOTTOM_ID	13
#define TOP_ID		14
#define LEFT_ID		15
#define RIGHT_ID	16

#define NUM_WATER_TEX 32
#define WATER_START_ID 17
#define WATER_END_ID WATER_START_ID + NUM_WATER_TEX

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_DEPTH 16

#pragma endregion

#pragma region Externs

extern UINT g_Texture[MAX_TEXTURES];

extern HDC g_DeviceHandle;
extern HGLRC g_GLContextHandle;
extern HINSTANCE g_WindowInstanceHandle;
extern HWND g_WindowHandle;
extern RECT g_Rect;
extern bool g_FullScreen;

extern float g_DeltaTime;
extern float g_WaterFlow;
extern float g_WaterHeight;
extern float g_WaterUV;

extern GCamera g_Camera;
extern Shader g_Shader;

extern BYTE g_HeightMap[MAP_SIZE * MAP_SIZE];
extern int g_TextureSize;

extern char dudvMap[260];
extern char normalMap[260];

#pragma endregion

#pragma region Function Prototypes

//************************************
// Method:    WinProc
// FullName:  WinProc
// Access:    public 
// Returns:   LRESULT CALLBACK
// Qualifier: Handles any events from the window
// Parameter: HWND hwnd
// Parameter: UINT message
// Parameter: WPARAM wParam
// Parameter: LPARAM lParam
//************************************
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//************************************
// Method:    MainLoop
// FullName:  MainLoop
// Access:    public 
// Returns:   WPARAM
// Qualifier: The main loop of our program
//************************************
WPARAM MainLoop();

#pragma endregion

#endif