#ifndef _WINDOW_H
#define _WINDOW_H

//************************************
// Method:    ChangeToFullScreen
// FullName:  ChangeToFullScreen
// Access:    public 
// Returns:   void
// Qualifier: Changes our screen to full screen
//************************************
void ChangeToFullScreen();

//************************************
// Method:    MakeWindow
// FullName:  MakeWindow
// Access:    public 
// Returns:   HWND
// Qualifier: A more extensible method for creating windows using Windows API
// Parameter: LPSTR windowName
// Parameter: int width
// Parameter: int height
// Parameter: DWORD style
// Parameter: bool fullScreen
// Parameter: HINSTANCE instance
//************************************
HWND MakeWindow(LPSTR windowName, int width, int height, DWORD style, bool fullScreen, HINSTANCE instance);

//************************************
// Method:    SetupPixelFormat
// FullName:  SetupPixelFormat
// Access:    public 
// Returns:   bool
// Qualifier: Sets up the piexel format for our window
// Parameter: HDC handle
//************************************
bool SetupPixelFormat(HDC handle);

//************************************
// Method:    SizeOpenGLScreen
// FullName:  SizeOpenGLScreen
// Access:    public 
// Returns:   void
// Qualifier: Sets the size of the viewport and the perspective matrix
// Parameter: int width
// Parameter: int height
//************************************
void SizeOpenGLScreen(int width, int height);

#endif