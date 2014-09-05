#ifndef _WATER_H
#define _WATER_H

//************************************
// Method:    AnimateNextFrame
// FullName:  AnimateNextFrame
// Access:    public 
// Returns:   bool
// Qualifier: Clamps animation to a frame rate
// Parameter: int desiredFrameRate
//************************************
bool AnimateNextFrame(int desiredFrameRate);

//************************************
// Method:    LoadAnimTextures
// FullName:  LoadAnimTextures
// Access:    public 
// Returns:   void
// Qualifier: Loads the animation textures into the array
// Parameter: UINT textureArray[]
// Parameter: LPSTR szFileName
// Parameter: int startIndex
// Parameter: int bitmapCount
//************************************
void LoadAnimTextures(UINT textureArray[], LPSTR szFileName, int startIndex, int bitmapCount);

//************************************
// Method:    CreateRenderTexture
// FullName:  CreateRenderTexture
// Access:    public 
// Returns:   void
// Qualifier: Renders the current scene to a texture map
// Parameter: UINT textureArray[]
// Parameter: int size
// Parameter: int channels
// Parameter: int type
// Parameter: int textureID
//************************************
void CreateRenderTexture(UINT textureArray[], int size, int channels, int type, int textureID);

//************************************
// Method:    RenderCaustics
// FullName:  RenderCaustics
// Access:    public 
// Returns:   void
// Qualifier: Cycles through the caustic maps to create the animation
// Parameter: float waterHeight
// Parameter: float causticScale
//************************************
void RenderCaustics(float waterHeight, float causticScale);

//************************************
// Method:    CreateReflectionTexture
// FullName:  CreateReflectionTexture
// Access:    public 
// Returns:   void
// Qualifier: Renders the relfection texture
// Parameter: float waterHeight
// Parameter: int textureSize
//************************************
void CreateReflectionTexture(float waterHeight, int textureSize);

//************************************
// Method:    CreateRefractionDepthTexture
// FullName:  CreateRefractionDepthTexture
// Access:    public 
// Returns:   void
// Qualifier: Renders the refraction and depth textures
// Parameter: float waterHeight
// Parameter: int textureSize
//************************************
void CreateRefractionDepthTexture(float waterHeight, int textureSize);

//************************************
// Method:    RenderWater
// FullName:  RenderWater
// Access:    public 
// Returns:   void
// Qualifier: Draws the quad for the water
// Parameter: float waterHeight
//************************************
void RenderWater(float waterHeight);

#endif