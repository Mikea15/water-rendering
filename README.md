#Water Rendering

This program uses normal maps to create the illusion of a highly tessellated surface of water. A full list of features is given below, along with
implementation details and an explanation of all the controls.

The project was inspired by [3]. All terrain textures and caustic textures were taken from the same source, with the exception of the normal and
dudv maps, which were generated using the tools described below, although the code was heavily adapted to fit a different design and implementation.
The tutorial was used as a guide to setup the 3D environment to better showcase the water effect, as well as a reference for the Windows API.

##Features
 - Ability to choose the normal and corresponding dudv map. There are currently 4 different normal and dudv map pairs included in the textures directory. 
   Additional maps can be generated using the TGAtoDOT3 and TGAtoDUDV programs found in the bin directory. 
 - Ability to manipulate the camera using first person view, including strafing.
 - Basic collision detection.
 - Ability to increase/decrease the water flow speed.
 - Ability to increase/decrease the foginess of the water bed.
 - Ability to simulate the water being 'closer' or 'further' away.
 - Ability to increase/decrease the level of detail on the terrain.
 - Ability to switch between wireframe and solid mode.
 - Ability to switch detail on or off.

##Controls
 - Mouse move: moves the camera
 - WASD/arrow keys: strafes and moves the camera up and down.
 - RMB: turns detail on/off the terrain
 - LMB: turns wireframe on/off
 - Space: Increases the level of detail. There are 7 possible levels of detail, and as soon as the last one is reached, it cycles back to the first one.
 - F1: increases the zoom on the water texture. This simulates the behaviour of the water if the camera was very far away.
 - F2: decreases the 'zoom' on the water texture. This simulates the behaviour of the water if the camera was very near.
 - F3: increases the speed of the water flow.
 - F4: decreases the speed of the water flow.

##Dependencies
 - Windows API
 - GLM math library
 - GLEW library

##References
[1] Mortveit, Henning, "Reflections from Bumpy Surfaces", in ShaderX3 Advanced Rendering with DirectX and OpenGL, First Edition. 
    Hingham, Massacusetts: Charles River Media, 2004, ch. 2, sec. 3, pp. 107-118.

[2] Kryachko, Yuri, "Using Vertex Texture Displacement for Realistic Water Rendering", in GPU Gems 2, First Edition.
    Addison-Wesley, 2005, ch. 18, pp. 283-294

[3] digiben, (2006, August 24) "Height Map Part6" [online]. Available at: http://www.gametutorials.com/tutorial/height-map-part6/

[4] Shoemake, Ken, "Arcball Rotation Control", in Graphics Gems IV, First Edition.
    AP Professional, ch. 3, sec. 1, pp. 175-192.

[5] OpenGL Tutorial Team, (2012, November 8) Tutorial 6 : Keyboard and Mouse [online]. Available at: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/

