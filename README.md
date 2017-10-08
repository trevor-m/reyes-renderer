# reyes-renderer
A REYES-style micropolygon renderer written in C++ which implements a subset of the RenderMan specification. Features include Monte Carlo motion blur, displacement and surface shaders using Perlin noise, transparency, and anti-aliasing.
For high resolution renders, the program must be compiled in 64-bit mode.

## Example Renders
<img width="160" alt="donuts" src="https://user-images.githubusercontent.com/12981474/31322156-5011623c-ac47-11e7-9052-c0998cd822ad.png">
<img width="320" alt="donuts_still" src="https://user-images.githubusercontent.com/12981474/31322157-502cf84e-ac47-11e7-81bc-60b5ea6b16ad.png">
<img width="200" alt="trevorshadertest" src="https://user-images.githubusercontent.com/12981474/31322159-50368a9e-ac47-11e7-96ce-b29d72d138ae.png">
<img width="200" alt="trevorshadertest_blur" src="https://user-images.githubusercontent.com/12981474/31322162-504ae066-ac47-11e7-9582-2e1cf6e0909a.png">
<img width="200" alt="earth" src="https://user-images.githubusercontent.com/12981474/31322161-503aefda-ac47-11e7-859c-d43a5a0be405.png">
<img width="200" alt="rocket" src="https://user-images.githubusercontent.com/12981474/31322160-503a0b9c-ac47-11e7-9b60-7081e15acb25.png">
<img width="200" alt="shadertest" src="https://user-images.githubusercontent.com/12981474/31322158-5034171e-ac47-11e7-8245-d2ad9fc78f0f.png">

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
