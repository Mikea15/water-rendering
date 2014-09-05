#ifndef _INIT_H
#define _INIT_H

//************************************
// Method:    CreateTexture
// FullName:  CreateTexture
// Access:    public 
// Returns:   bool
// Qualifier: Creates a texture and binds it 
// Parameter: UINT &textureID
// Parameter: LPSTR filename
//************************************
bool CreateTexture(UINT &textureID, LPSTR filename);

//************************************
// Method:    InitializeOpenGL
// FullName:  InitializeOpenGL
// Access:    public 
// Returns:   void
// Qualifier: Initializes the OpenGL context
// Parameter: int width
// Parameter: int height
//************************************
void InitializeOpenGL(int width, int height);

//************************************
// Method:    Init
// FullName:  Init
// Access:    public 
// Returns:   void
// Qualifier: Initializes OpenGL and loads the textures into memory
// Parameter: HWND handle
//************************************
void Init(HWND handle);

//************************************
// Method:    DeInit
// FullName:  DeInit
// Access:    public 
// Returns:   void
// Qualifier: Frees any allocated memory before shutting the program
//************************************
void DeInit();

#endif