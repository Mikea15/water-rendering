#ifndef _TERRAIN_H
#define _TERRAIN_H

#define MAP_SIZE 1024
#define STEP_SIZE 16

//************************************
// Method:    Height
// FullName:  Height
// Access:    public 
// Returns:   int
// Qualifier: Returns the height of a height map given x and y
// Parameter: BYTE *heightMap
// Parameter: int X
// Parameter: int Y
//************************************
int Height(BYTE *heightMap, int X, int Y);

//************************************
// Method:    LoadRawFile
// FullName:  LoadRawFile
// Access:    public 
// Returns:   void
// Qualifier: Loads a raw file into memory
// Parameter: LPSTR filename
// Parameter: int size
// Parameter: BYTE *heightMap
//************************************
void LoadRawFile(LPSTR filename, int size, BYTE *heightMap);

//************************************
// Method:    RenderHeightMap
// FullName:  RenderHeightMap
// Access:    public 
// Returns:   void
// Qualifier: Turns a height map into primitives and draws them
// Parameter: BYTE *heightMap
//************************************
void RenderHeightMap(BYTE *heightMap);

#endif