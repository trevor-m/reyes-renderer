// Scenes cpp file

#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>

/********************************************/
/********************************************/
// Texture lookup
void TextureMap0()
{
	texture(0);
}

/********************************************/
/********************************************/
void TransparencyTest()
{
	RiBegin(RI_NULL);
	//RiFormat(960, 720, 1.0);
	RiFormat(400, 300, 1.0);
	//RiFormat(200, 150, 1.0);
	RiDisplay("TransparencyTest.png", "", "");
	RiPixelSamples(2, 2);

	RiFrameBegin(0);
	/* set the perspective transformation */
	float fov = 45.0;
	RiProjection(RI_PERSPECTIVE, "fov", &fov);
	//RiProjection(RI_ORTHOGRAPHIC);

	RiWorldBegin();
	RiTransformBegin();
	RtColor color = { 1,0,0 };
	RtColor opacity = { .4,.4,.4 };
	RiOpacity(opacity);
	RiColor(color);
	RiTranslate(0, 0.5, 7.0);
	RiRotate(60, 1, 0, 0);
	RiTorus(1, .25, 0, 250, 270);
	RiTransformEnd();
	RiTransformBegin();
	color[0] = 0; color[1] = 1;
	opacity[0] = 0.4; opacity[1] = 0.4; opacity[2] = 0.4;
	RiOpacity(opacity);
	RiColor(color);
	RiTranslate(0, 0, 8.0);
	RiRotate(60, 1, 0, 0);
	RiRotate(30, 0, 1, 0);
	RiCylinder(1, -1, 1, 300);
	RiTransformEnd();
	RiTransformBegin();
	color[1] = 0; color[2] = 1;
	opacity[0] = .7; opacity[1] = .7; opacity[2] = .7;
	RiOpacity(opacity);
	RiColor(color);
	RiTranslate(0, 1, 9.0);
	RiRotate(60, 1, 0, 0);
	RiSphere(1.8, -1.0, 1.0, 250);
	RiTransformEnd();
	RiTransformBegin();
	color[0] = 1; color[1] = .6; color[2] = .6;
	RiColor(color);
	opacity[0] = .6; opacity[1] = .6; opacity[2] = .6;
	RiOpacity(opacity);
	RiTranslate(0, -1, 8.5);
	RiRotate(-160, 1, 0, 0);
	RiRotate(30, 0, 1, 0);
	RiRotate(140, 0, 0, 1);
	RiCone(2.5, 1, 270);
	RiTransformEnd();
	RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

/********************************************/
/********************************************/
void ShaderTest()
{
	RiBegin(RI_NULL);
	//RiFormat(960, 720, 1.0);
	RiFormat(400, 300, 1.0);
	//RiFormat(200, 150, 1.0);
	RiDisplay("ShaderTest.png", "", "");
	RiPixelSamples(4, 4);

	RiFrameBegin(0);
	/* set the perspective transformation */
	float fov = 45.0;
	RiProjection(RI_PERSPECTIVE, "fov", &fov);
	//RiProjection(RI_ORTHOGRAPHIC);

	RiWorldBegin();
	RiTransformBegin();
	RtColor color = { 1,0,0 };
	RtColor opacity = { .4,.4,.4 };
	RiOpacity(opacity);
	RiColor(color);
	CHECK_SIZE_X = 20;
	CHECK_SIZE_Y = 10;
	RiSurface(CHECKERBOARD);
	RiTranslate(0, 0.5, 11);
	RiRotate(70, 0, 1, 0);
	RiTorus(2.5, .5, 0, 360, 360);
	RiTransformEnd();
	RiTransformBegin();
	color[0] = 0; color[1] = 1; color[2] = 0;
	opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
	RiOpacity(opacity);
	RiSurface(NULL);
	RiColor(color);
	RiTranslate(3, -1.5, 11);
	RiRotate(90, 1, 0, 0);
	RiCylinder(1, -1, 1, 360);
	RiTransformEnd();
	RiTransformBegin();
	opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
	RiOpacity(opacity);
	RiMakeTexture("earth.jpg", 0);
	void(*earthShader)(void) = TextureMap0;
	RiSurface(earthShader);
	BUMP_AMPLITUDE = 1;
	BUMP_MIN_FREQ_EXP = 2;
	BUMP_MAX_FREQ_EXP = 6;
	RiDisplacement(BUMPY);
	RiTranslate(3, 1, 11.0);
	RiRotate(-40, 0, 0, 1);
	RiRotate(-100, 0, 1, 0);
	RiRotate(110, 1, 0, 0);
	RiSphere(1, -1.5, 1.5, 360);
	RiTransformEnd();
	RiTransformBegin();
	opacity[0] = .8; opacity[1] = .8; opacity[2] = .8;
	RiOpacity(opacity);
	RiSurface(earthShader);
	RiDisplacement(NULL);
	RiTranslate(-2.5, 1, 11.0);
	RiRotate(-175, 0, 1, 0);
	RiRotate(110, 1, 0, 0);
	RiSphere(1.5, -1.5, 1.5, 360);
	RiTransformEnd();
	RiTransformBegin();
	color[0] = 0.4; color[1] = 0.2; color[2] = 0;
	RiColor(color);
	opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
	RiOpacity(opacity);
	RiSurface(NULL);
	BUMP_AMPLITUDE = .5;
	BUMP_MAX_FREQ_EXP = 8;
	RiDisplacement(BUMPY);
	RiTranslate(-2.5, -2.5, 11);
	RiRotate(-90, 1, 0, 0);
	RiRotate(90, 0, 0, 1);
	RiCone(2.5, 1, 360);
	RiTransformEnd();
	RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

/********************************************/
/********************************************/
void Rocket()
{
	RiBegin(RI_NULL);
	//RiFormat(960, 720, 1.0);
	RiFormat(400, 300, 1.0);
	//RiFormat(200, 150, 1.0);
	RiDisplay("Rocket.png", "", "");
	RiPixelSamples(2, 2);

	RiFrameBegin(0);
	/* set the perspective transformation */
	float fov = 45.0;
	RiProjection(RI_PERSPECTIVE, "fov", &fov);
	//RiProjection(RI_ORTHOGRAPHIC);

	RiWorldBegin();
	RiTransformBegin();
	// water
	RtColor color = { 0,0,1 };
	RtColor opacity = { .6,.6,.6 };
	RiOpacity(opacity);
	RiColor(color);
	BUMP_AMPLITUDE = .05;
	BUMP_MIN_FREQ_EXP = 2;
	BUMP_MAX_FREQ_EXP = 6;
	RiDisplacement(BUMPY);
	RiScale(7, 1.15, 1);
	RiTranslate(0, -0.75, 4);
	RiRotate(180, 0, 1, 0);
	RiRotate(110, 1, 0, 0);
	RiTranslate(0, 0, 1);
	RiRotate(180, 1, 0, 0);
	RiSphere(1, .9, 1, 360);
	// mountain
	color[0] = 0.6; color[1] = 0.3; color[2] = 0;
	opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
	RiColor(color);
	RiOpacity(opacity);
	RiDisplacement(NULL);
	BUMP_AMPLITUDE = 2;
	BUMP_MIN_FREQ_EXP = 3;
	BUMP_MAX_FREQ_EXP = 9;
	RiDisplacement(BUMPY);
	RiTranslate(0, 4, -6);
	RiRotate(15, 1, 0, 0);
	RiSphere(7, 6.8, 7, 360);
	RiTransformEnd();

	RiTransformBegin();
	// rocket
	RiDisplacement(NULL);
	color[0] = .8; color[1] = 0.8; color[2] = .8;
	RiColor(color);
	RiTranslate(-.25, .25, 2);
	RiRotate(40, 0, 0, 1);
	RiScale(1, 2, 1);
	RiRotate(150, 0, 1, 0);
	RiRotate(-90, 1, 0, 0);
	RiSphere(.1, -.1, .1, 360);

	RiTranslate(0, 0, .07);
	RiCone(.075, .075, 360);

	RiSurface(NULL);
	BUMP_AMPLITUDE = .1;
	BUMP_MIN_FREQ_EXP = 5;
	BUMP_MAX_FREQ_EXP = 7;
	RiDisplacement(BUMPY);
	color[0] = 1; color[1] = 0.27; color[2] = 0;
	opacity[0] = .4; opacity[1] = .4; opacity[2] = .4;
	RiOpacity(opacity);
	RiColor(color);
	RiTranslate(0, 0, -.45);
	RiCone(.3, .1, 360);

	color[1] = .1;
	RiColor(color);
	opacity[0] = 1; opacity[1] = 1; opacity[2] = 1;
	RiOpacity(opacity);
	RiDisplacement(NULL);
	RiScale(20, .5, 1);
	RiTranslate(0, 0, .375);
	RiSphere(.025, -.025, .025, 360);

	RiScale(1 / 40, 16, 1);
	RiTranslate(0, -0.015, -.075);
	RiSphere(.025, -.025, .025, 360);
	RiTransformEnd();
	RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

/********************************************/
/********************************************/
void Tunnel()
{
	RiBegin(RI_NULL);
	//RiFormat(960, 720, 1.0);
	RiFormat(400, 300, 1.0);
	RiDisplay("Tunnel.png", "", "");
	RiPixelSamples(2, 2);

	RiFrameBegin(0);
	/* set the perspective transformation */
	float fov = 45.0;
	RiProjection(RI_PERSPECTIVE, "fov", &fov);
	//RiProjection(RI_ORTHOGRAPHIC);

	RiWorldBegin();
	RiTransformBegin();
	RtColor color = { 1,0,0 };
	RiColor(color);
	RiTranslate(0, 0.5, 7.0);
	RiRotate(60, 1, 0, 0);
	RiTorus(1, .25, 0, 360, 360);
	RiTransformEnd();
	RiTransformBegin();
	color[0] = 0; color[1] = 1;
	RiColor(color);
	RiTranslate(0, 0, 8.0);
	RiRotate(60, 1, 0, 0);
	RiRotate(30, 0, 1, 0);
	RiCylinder(1, -1, 1, 360);
	RiTransformEnd();
	RiTransformBegin();
	color[1] = 0; color[2] = 1;
	RiColor(color);
	RiTranslate(0, 1, 9.0);
	RiRotate(60, 1, 0, 0);
	RiSphere(1.0, -1.0, 1.0, 360);
	RiTransformEnd();
	RiTransformBegin();
	color[0] = 1; color[1] = .4; color[2] = .4;
	RiColor(color);
	CHECK_SIZE_X = 40;
	CHECK_SIZE_Y = 40;
	RiSurface(CHECKERBOARD);
	RiTranslate(0, -1, 8.5);
	RiRotate(-160, 1, 0, 0);
	RiRotate(30, 0, 1, 0);
	RiCone(2, 1, 360);
	RiTransformEnd();
	RiTransformBegin();
	CHECK_SIZE_X = 40;
	CHECK_SIZE_Y = 40;
	RiTranslate(0, 0, 7.0);
	RiCylinder(3, 0, 10, 360);
	RiTransformEnd();
	RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

/********************************************/
/********************************************/
void Earth()
{
	RiBegin(RI_NULL);
	//RiFormat(960, 720, 1.0);
	RiFormat(400, 300, 1.0);
	//RiFormat(200, 150, 1.0);
	RiDisplay("Earth.png", "", "");
	RiPixelSamples(2, 2);

	RiFrameBegin(0);
	/* set the perspective transformation */
	float fov = 45.0;
	RiProjection(RI_PERSPECTIVE, "fov", &fov);
	//RiProjection(RI_ORTHOGRAPHIC);

	RiWorldBegin();
	RiTransformBegin();
	RiMakeTexture("earth.jpg", 0);
	void(*earthShader)(void) = TextureMap0;
	RiSurface(earthShader);
	RtColor blue = { 0, 0, 1 };
	RtColor opacity = { .9, .9, .9 };
	RiColor(blue);
	RiOpacity(opacity);
	BUMP_AMPLITUDE = .02;
	BUMP_MIN_FREQ_EXP = 14;
	BUMP_MAX_FREQ_EXP = 16;
	RiDisplacement(BUMPY);
	RiTranslate(0, 0, 5.0);
	RiRotate(-175, 0, 1, 0);
	RiRotate(110, 1, 0, 0);
	RiSphere(1, -1, 1, 360);
	RiTransformEnd();
	RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

/********************************************/
/********************************************/
void Tornado()
{
	RiBegin(RI_NULL);
	//RiFormat(960, 720, 1.0);
	RiFormat(400, 300, 1.0);
	//RiFormat(200, 150, 1.0);
	RiDisplay("Tornado.png", "", "");
	RiPixelSamples(2, 2);

	RiFrameBegin(0);
	/* set the perspective transformation */
	float fov = 45.0;
	RiProjection(RI_PERSPECTIVE, "fov", &fov);
	//RiProjection(RI_ORTHOGRAPHIC);

	RiWorldBegin();
	RiTransformBegin();
	RtColor gray = { .4, .4, .4 };
	RtColor opacity = { 0.5, 0.5, 0.5 };
	RiColor(gray);
	RiOpacity(opacity);
	BUMP_AMPLITUDE = 1;
	BUMP_MIN_FREQ_EXP = 3;
	BUMP_MAX_FREQ_EXP = 10;
	RiDisplacement(BUMPY);
	RiTranslate(0, 0, 5.0);
	RiRotate(90, 0, 1, 0);
	RiRotate(80, 1, 0, 0);
	RiTranslate(0, 0, -1.5);
	RiCone(3, 1, 360);
	RiTransformEnd();
	RiWorldEnd();

	RiFrameEnd();

	RiEnd();
}

/********************************************/
/********************************************/
void SampleScene1(void) {
	int i;
	int nf;
	float slopex, slopey, slopez;
	char name[50];

	RtColor red = { 1,0,0 };
	RtColor green = { 0,1,0 };
	RtColor blue = { 0,0,1 };
	RtColor white = { 1,1,1 };


	RtPoint p1 = { 30,0,10 }; /* ball's initial position */
	RtPoint p2 = { 0,20,10 }; /* ball's final position  */


	RtFloat fov = 45;
	RtFloat intensity1 = 0.1;
	RtFloat intensity2 = 1.5;
	RtInt init = 0, end = 1;


	nf = 100; /* number of frames to output */
	slopex = (p2[0] - p1[0]) / nf;
	slopey = (p2[1] - p1[1]) / nf;
	slopez = (p2[2] - p1[2]) / nf;

	RiBegin(RI_NULL);
	RiFormat(320, 240, 1);
	RiPixelSamples(2, 2);
	RiShutter(0, 1);

	/* loop through all the frames */
	for (i = 1; i <= nf; i++) {
		RiFrameBegin(i);
		sprintf_s(name, "samplescene/image_%02d.bmp", i - 1);
		RiDisplay(name, "file", "rgb", RI_NULL);

		RiProjection("perspective", "fov", &fov, RI_NULL);
		RiTranslate(0, -5, 60);
		RiRotate(-120, 1, 0, 0);
		RiRotate(25, 0, 0, 1);

		RiWorldBegin();
		RiColor(blue);
		RiTransformBegin();
		RiCylinder(1, 0, 20, 360, RI_NULL);
		RiTranslate(0, 0, 20);
		RiCone(2, 2, 360, RI_NULL);
		RiTransformEnd();

		RiColor(green);
		RiTransformBegin();
		RiRotate(-90, 1, 0, 0);
		RiCylinder(1, 0, 20, 360, RI_NULL);
		RiTranslate(0, 0, 20);
		RiCone(2, 2, 360, RI_NULL);
		RiTransformEnd();

		RiColor(red);
		RiTransformBegin();
		RiRotate(90, 0, 1, 0);
		RiCylinder(1, 0, 20, 360, RI_NULL);
		RiTranslate(0, 0, 20);
		RiCone(2, 2, 360, RI_NULL);
		RiTransformEnd();


		RiColor(white);
		RiTransformBegin();
		RiTranslate(p1[0] + slopex * (i - 1), p1[1] + slopey * (i - 1), p1[2] + slopez * (i - 1));
		RiSphere(5, -5, 5, 360, RI_NULL);
		RiTransformEnd();
		RiWorldEnd();

		/* when you hit this command you should output the final image for this frame */
		RiFrameEnd();
	}
	RiEnd();
}

/********************************************/
/********************************************/
// MY SCENES


void Donuts() {
	RiBegin(RI_NULL);
	RiFormat(320, 240, 1.0);
	RiPixelSamples(8, 8);
	//RiPixelSamples(4, 4);
	RiFrameAspectRatio(1.0f);
	RiDisplay("Donuts.bmp", "", "");
	RiFrameBegin(0);
	/* set the perspective transformation */
	float fov = 45.0;
	RiProjection(RI_PERSPECTIVE, "fov", &fov);

	RiWorldBegin();

	srand(19);
	for (int i = 0; i < 12; i++) {
		float xpos = (i % 4) * 1.4 - (1.4*1.5);
		float ypos = (i % 3)* 1.3 - 0.5f;
		float zpos = 7 + ((float)rand() / RAND_MAX) * 7 - 2.5;
		float xrot = ((float)rand() / RAND_MAX) * 180; //70;
		float yrot = ((float)rand() / RAND_MAX) * 180; //70;
		float zrot = ((float)rand() / RAND_MAX) * 360; //35;
		RiTransformBegin();
			RiMotionBlur(((float)rand() / RAND_MAX) * 0.4 - 0.2f, ((float)rand() / RAND_MAX) * 0.3f + 0.7f, 0);
			RiDisplacement(DONUT_DISPLACEMENT);
			RiSurface(DONUT_SURFACE);
			RiTranslate(xpos, ypos, zpos);
			RiRotate(zrot, 0, 0, 1);
			RiRotate(yrot, 0, 1, 0);
			RiRotate(xrot, 1, 0, 0);
			RiTorus(0.5f, 0.3f, 0, 360, 360);
		RiTransformEnd();
	}
	RiMotionBlur(0, 0, 0);
	RiTransformBegin();
		BUMP_AMPLITUDE = 0.05;
		BUMP_MIN_FREQ_EXP = 3;
		BUMP_MAX_FREQ_EXP = 8;
		LIGHTING_REFLECTIVITY = 2.0f;
		LIGHTING_SPEC_ATTEN = 0.3f;
		RiDisplacement(BUMPY);
		RiSurface(WOOD);
		RiTranslate(0, 0, 12);
		RiRotate(90, 1, 0, 0);
		RiRotate(180, 0, 1, 0);
		RiCylinder(8, -3, 8.5, 180);
	RiTransformEnd();

	RiTransformBegin();
		//RiDisplacement(NULL);
		LIGHTING_REFLECTIVITY = 64.0f;
		LIGHTING_SPEC_ATTEN = 1.0f;
		RiSurface(MARBLE);
		RiTranslate(0, -12, 13);
		RiRotate(-90, 1, 0, 0);
		RiScale(1, 1, 0.5f);
		RiSphere(20, 17, 20, 360);
	RiTransformEnd();
	RiWorldEnd();
	RiFrameEnd();
	RiEnd();
}

// Create and render a scene that has the four main primitives (sphere, cylinder, cone, torus),
// but set the zmin, zmax, and thetamax for these to different values so that the shapes are
// incomplete and are missing their top and bottom and are open on one side
void MyTestScene() {
	RtColor red = { 0.9, 0.1, 0.1 };
	RtColor green = { 0.2, 0.9, 0.3 };
	RtColor blue = { 0.1, 0.2, 0.9 };
	RtColor purple = { 0.9, 0.1, 0.8 };
	RtColor transparent = { 0.5, 0.5, 0.5 };
	RiBegin(RI_NULL);
	RiFormat(400, 300, 1.0);
	RiPixelSamples(2, 2);
	RiFrameAspectRatio(1.0f);
	RiDisplay("TrevorTestScene.bmp", "", "");
	RiFrameBegin(0);
		/* set the perspective transformation */
		float fov = 45.0;
		RiProjection(RI_PERSPECTIVE, "fov", &fov);
		RiWorldBegin();
			RiTransformBegin();
				RiOpacity(transparent);
				RiColor(red);
				RiTranslate(-1, 0, 8);
				RiRotate(110, 1, 0, 0);
				RiSphere(2, -1.5, 1.5, 250.0f);
			RiTransformEnd();
			RiTransformBegin();
				RiColor(green);
				RiTranslate(1, 0, 8);
				RiRotate(180, 0, 1, 0);
				RiRotate(110, 1, 0, 0);
				RiRotate(45, 0, 0, 1);
				RiCylinder(2, -1, 1, 250.0f);
			RiTransformEnd();
			RiTransformBegin();
				RiColor(purple);
				RiTranslate(0, 1, 8);
				RiRotate(-45, 1, 0, 0);
				RiTorus(2, 0.4, 60, 180, 250);
			RiTransformEnd();
			RiTransformBegin();
				RiColor(blue);
				RiTranslate(-1, -1, 8);
				RiRotate(-120, 1, 0, 0);
				RiRotate(180, 0, 0, 1);
				RiCone(2.5, 1, 250.0f);
			RiTransformEnd();
		RiWorldEnd();
	RiFrameEnd();
	RiEnd();
}

// Create and render a test scene that shows off your different shaders (standard lighting, checkerboard, bumpy)
void MyShaderTest() {
	RtColor grass = { 0, 123.0f / 255, 12.0f / 255 };
	RtColor post = { 0.7,0.6, 0 };
	RtColor transparent = { 0.4, 0.4, 0.4 };
	RtColor opaque = { 1, 1, 1 };
	RtColor sky = { 135.0f / 255, 206.0f / 255, 235.0f / 255 };
	RiBegin(RI_NULL);
	RiFormat(400, 300, 1.0);
	RiPixelSamples(4, 4);
	RiFrameAspectRatio(1.0f);
	RiDisplay("TrevorShaderTest.bmp", "", "");
	//RiBackgroundColor(sky);
	RiFrameBegin(0);
		/* set the perspective transformation */
		float fov = 45.0;
		RiProjection(RI_PERSPECTIVE, "fov", &fov);
		RiWorldBegin();
			// ball
			//RiMotionBlur(0.2, 0.8, 0.3);
			RiTransformBegin();
				CHECK_SIZE_X = 10;
				CHECK_SIZE_Y = 5;
				RiSurface(CHECKERBOARD);
				RiTranslate(-1.5, -0.7, 8.8);
				RiRotate(110, 1, 0, 0);
				RiRotate(45, 0, 1, 0);
				RiSphere(0.8, -0.8, 0.8, 360.0f);
			RiTransformEnd();
			//RiMotionBlur(0, 0, 0);
			// goal
			RiTransformBegin();
				CHECK_SIZE_X = 60;
				CHECK_SIZE_Y = 60;
				RiSurface(CHECKERBOARD);
				RiOpacity(transparent);
				RiTranslate(0, 0.5, 13);
				RiScale(1, 1, 0.3f);
				RiRotate(90, 1, 0, 0);
				RiRotate(90, 0, 1, 0);
				RiCylinder(2, -2, 2, 100);
			RiTransformEnd();
			//goal posts
			RiTransformBegin();
				RiSurface(NULL);
				RiOpacity(opaque);
				RiColor(post);
				RiTranslate(2, 0.5, 13);
				RiRotate(-90, 1, 0, 0);
				RiCone(2, 0.3, 360);
			RiTransformEnd();
			RiTransformBegin();
				RiTranslate(-2, 0.5, 13);
				RiRotate(-90, 1, 0, 0);
				RiCone(2, 0.3, 360);
			RiTransformEnd();
			// grass
			RiTransformBegin();
				RiOpacity(opaque);
				RiColor(grass);
				RiSurface(NULL);
				BUMP_AMPLITUDE = 0.8;
				BUMP_MIN_FREQ_EXP = 10;
				BUMP_MAX_FREQ_EXP = 19;
				RiDisplacement(BUMPY);
				RiTranslate(0, -9, 15);
				RiRotate(-95, 1, 0, 0);
				RiSphere(10, 5, 10, 360.0f);
			RiTransformEnd();
		RiWorldEnd();
	RiFrameEnd();
	RiEnd();
}

void main() {
	//SampleScene1();
	//TransparencyTest();
	//ShaderTest();
	//Rocket();
	//Tunnel();
	//Earth();
	//Tornado();

	//my scenes
	//MyTestScene();
	MyShaderTest();
	//Donuts();
}