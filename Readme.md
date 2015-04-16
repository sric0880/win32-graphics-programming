#Win32 Graphics Programming Learning
##Features
1. Transform Matrix:
  * Local Transform(Translation, Rotation and Scale)
  * Camera Transform
  * Projection Transform(Orth and Perspective)
2. Framebuffer and Depthbuffer support
3. Draw line(Bresenham Algorithm) and draw triangle support
4. Lighting system:
  * Direction Light
  * Ambient, Specular and Diffuse color support
  * Shininess support
  * Vertex Lit
  * Per Pixel Lit (normal interpolation)
5. Backface culling
6. Triangle clipping
7. Triangle scan line algorithm (Rasterization)
8. Perspective Texture Mapping
9. Depth Test
10. Camera control using keyboard and mouse
11. Frame update and refresh 60 times per second
12. Render object auto-rotation
13. Example:
  * Cube
  * Quad with texture
  * Sphere with texture

##System requirement
* Visual Studio 2013 utimate with update 4
* window 7 or above

##Bugs
1. [Fixed] After porj-model-view transform, x,y,z must not divided by w before triangle clipping.
2. Cannot get the accurate windows resolution in virtual machine.
3. [Fixed] Texture coordinate interpolation while clipping.
4. [Fixed] Depth buffer value < 0.
5. Blink while scaling window or moving window.
6. [Fixed] Back face culling error.
7. [Fixed] Interpolated fragment out of triangle.
8. [Fixed] Clipping triangle while orth projection.
9. [Fixed] Buffer overflow.