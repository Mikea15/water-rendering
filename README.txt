*******************
* Water Rendering *
*******************
This program uses normal maps to create the illusion of a highly tessellated surface of water. A full list of features is given below, along with
implementation details and an explanation of all the controls.

The project was inspired by [3]. All terrain textures and caustic textures were taken from the same source, with the exception of the normal and
dudv maps, which were generated using the tools described below, although the code was heavily adapted to fit a different design and implementation.
The tutorial was used as a guide to setup the 3D environment to better showcase the water effect, as well as a reference for the Windows API.


************
* Features *
************
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

**************************
* Implementation Details *
**************************
 - Water Rendering:
   The water primitive is a simple quad that exists in the middle of the terrain. The water effect is created inside the fragment shader by taking the inputs from the 
   normal and dudv maps to compute the way that ligth bounces off the surface. This is done with a combination of a partial version of Schlick's approximation 
   of the Fresnel term and the techniques outlined in [1]. It is important to note that there is no processing done at the program level, the entirety of the effect
   is created inside the fragment shader. Further details below.

   We first take a seamless (or reasonably seamless) image of water and use TGAtoDOT3 to generate the normal map. This in turn is used by TGAtoDUDV to create the dudv map,
   which essentially contains the derivatives of the normals described in the normal map. These two are then taken by the program and mapped as textures, which are then
   sent to the fragment shader as uniform variables. Inside the shader, we compute the direction that the light (which has been transformed into tangent space 
   coordinates by the vertex shader) bounces of the surface, refraction, reflection, depth, and a compressed version of the Fresnel term (which is based on Schlick's approximation).
   Finally we compute the specular hightlight and use all this to calculate the colour for that fragment.

 - Caustics:
   The caustics that are seen in the program are simply the result of cycling through the caustic textures found in the textures folder and mapping them on to the
   water bed. These are only displayed whenever the camera crosses the plane of water. To further enhance the effect of an underwater world, we use fog to give
   everything a blue tint and to blur things that are further away. Note that no computations are made to actually generate the caustics based on the behaviour of the
   water surface above. They are just an animation of the textures that are cycled through.

- Camera:
  The camera movement was partially inspired by 4, although it was adapted from a standard arcball into a first person view camera following the techniques outlined in [5].

************
* Controls *
************
 - Mouse move: moves the camera
 - WASD/arrow keys: strafes and moves the camera up and down.
 - RMB: turns detail on/off the terrain
 - LMB: turns wireframe on/off
 - Space: Increases the level of detail. There are 7 possible levels of detail, and as soon as the last one is reached, it cycles back to the first one.
 - F1: increases the zoom on the water texture. This simulates the behaviour of the water if the camera was very far away.
 - F2: decreases the 'zoom' on the water texture. This simulates the behaviour of the water if the camera was very near.
 - F3: increases the speed of the water flow.
 - F4: decreases the speed of the water flow.

****************
* Dependencies *
****************
 - Windows API
 - GLM math library
 - GLEW library

****************
* Observations *
****************
 - The sampling technique outlined in [2] could not be implemented due to the different ways in which the water is created. An adaptation was tried, in which two normal
   maps were blended using Photoshop and the resulting dudv map was created. Unfortunately the resulting effect was not as pleasing, and so the idea was scrapped. The
   original tga and bmp files are still included in case the user wishes to view the results.

 - A full Fresnel term was implemented into the shaders, however this resulted in the water loosing a bit of its transparency and in weird artifacts where the light
   hit the water. A better approach may be tried later on, but for the time being the implementation was left as is.

**************
* References *
**************
[1] Mortveit, Henning, "Reflections from Bumpy Surfaces", in ShaderX3 Advanced Rendering with DirectX and OpenGL, First Edition. 
    Hingham, Massacusetts: Charles River Media, 2004, ch. 2, sec. 3, pp. 107-118.
[2] Kryachko, Yuri, "Using Vertex Texture Displacement for Realistic Water Rendering", in GPU Gems 2, First Edition.
    Addison-Wesley, 2005, ch. 18, pp. 283-294
[3] digiben, (2006, August 24) "Height Map Part6" [online]. Available at: http://www.gametutorials.com/tutorial/height-map-part6/
[4] Shoemake, Ken, "Arcball Rotation Control", in Graphics Gems IV, First Edition.
    AP Professional, ch. 3, sec. 1, pp. 175-192.
[5] (2012, November 8) Tutorial 6 : Keyboard and Mouse [online]. Available at: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
