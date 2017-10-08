# reyes-renderer
A REYES-style micropolygon renderer written in C++ which implements a subset of the RenderMan specification. Features include Monte Carlo motion blur, displacement and surface shaders using Perlin noise, transparency, and anti-aliasing.
For high resolution renders, the program must be compiled in 64-bit mode.

## Example Renders

## Scenes
The scenes are all contained within SampleScene.cpp. The main() function, also defined in this file, can be used to determine which scenes to render.

## RenderMan Spec
The following RenderMan functions defined in Ri.h are implemented:
* RiBegin
* RiEnd
* RiFormat
* RiProjection
* RiFrameAspectRatio
* RiPixelSamples
* RiDisplay
* RiFrameBegin
* RiFrameEnd
* RiWorldBegin
* RiWorldEnd
* RiTransformBegin
* RiTransformEnd
* RiIdentity
* RiTransform
* RiPerspective
* RiTranslate
* RiRotate
* RiScale
* RiConcactTransform
* RiSphere
* RiCone
* RiCylinder
* RiTorus
* RiColor
* RiOpacity
* RiBackgroundColor - new
* RiMakeTexture - new
* RiSurface
* RiDisplacement
* RiShutter
* RiMotionBlur - new

## Shaders
Shaders are defined in Shader.h and implemented in Shader.cpp.

### Surface Shaders
* texture
* LIGHTING
* CHECKERBOARD
* MARBLE
* WOOD
* DONUT_SURFACE

### Displacement Shaders
* BUMPY
* DONUT_DISPLACEMENT

## Motion Blur
My implementation of motion blur allows for a single translation movement per frame only.
To use motion blur, I created RiMotionBlur(dx, dy, dz) which allows you to specify the movement of subsequent primitives. RiShutter specifies the capture start and stop time (usually 0 and 1).
To turn motion blur off, RiMotionBlur(0, 0, 0) should be sufficient.
I did not optimize the motion blur much, so for large displacements the bounding box of each micropolygon can be quite big, causing significant render times.
An example of motion blur can be seen in the 'Donuts' scene.
