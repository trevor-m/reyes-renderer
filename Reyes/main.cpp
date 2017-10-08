#include "Ri.h"
#include "Shader.h"
#include <stdlib.h>

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

void myScene(void) {
	RtColor gemColor = { 0.25, 0.5, 1 };
	RtColor ringColor = { 1, 1, 0 };
	RtColor transparent = { 0.5, 0.5, 0.5 };
	RtColor opaque = { 1.0, 1.0, 1.0 };
	RiMakeTexture("earth.jpg", 0);

	RiBegin(RI_NULL);
		RiFormat(300, 300, 1.0);
		RiPixelSamples(4, 4);
		RiFrameAspectRatio(4.0/3.0);
		RiDisplay("scene.bmp", "", "");
		RiFrameBegin(0);
			/* set the perspective transformation */
			float fov = 45.0;
			RiProjection(RI_PERSPECTIVE, "fov", &fov);
			
			RiWorldBegin();
				BUMP_AMPLITUDE = 1;
				BUMP_MIN_FREQ_EXP = 6;
				BUMP_MAX_FREQ_EXP = 9;
				RiDisplacement(BUMPY);
				RiTransformBegin();
					RiTranslate(-1.6, 0, 10);
					RiColor(gemColor);
					RiSphere(1.0, -1.0, 1.0, 360);
				RiTransformEnd();
				
				RiOpacity(transparent);
				RiTransformBegin();
					
					RiTranslate(1, 0, 10);
					RiRotate(45, 1, 0, 0);
					RiColor(ringColor);
					RiTorus(2, 0.5, 0, 360, 360);
				RiTransformEnd();
				RiDisplacement(NULL);
				//CHECK_SIZE_X = 40.0f;
				//CHECK_SIZE_Y = 40.0f;
				RiSurface(NOISY);
				RiOpacity(opaque);
				RiTransformBegin();
					
					RiTranslate(0, 5, 13);
					RiRotate(120, 1, 0, 0);
					RiCylinder(5, -5, 5, 150);
				RiTransformEnd();
			RiWorldEnd();
		RiFrameEnd();
	RiEnd();
}

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
	BUMP_AMPLITUDE = 0.1f;
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

//int main() {
	//Earth();
	//TransparencyTest();
	//myScene();
	//Rocket();
	//SampleScene1();

	//return 0;
//}