#include "Shader.h"
#include "Vector4.h"
#include "Noise.h"
#include <algorithm>
#include <cmath>
using std::max;
using std::min;

// Useful globals
Noise noise;

// Useful functions
float mix(float a, float b, float t) {
	return a + t * (b - a);
}

void mix(RtColor a, RtColor b, float t, RtColor out) {
	for (int i = 0; i < 3; i++)
		out[i] = a[i] + t * (b[i] - a[i]);
}


float MARBLE_PERIOD_X = 1;
float MARBLE_PERIOD_Y = 3;
float MARBLE_TURBULENCE = 40;
void MARBLE() {
	//create a tubulent sine wave
	float turbulence = MARBLE_TURBULENCE * noise.octaveNoise(__my_shader_u, __my_shader_v, 0, 2, 9);
	float sine = abs(sin(__my_shader_u * MARBLE_PERIOD_X  + __my_shader_v * MARBLE_PERIOD_Y + turbulence));
	
	//blue veins
	float turbulence2 = MARBLE_TURBULENCE * 2 * noise.octaveNoise(__my_shader_u, __my_shader_v, 3, 1, 10);
	float sine2 = abs(sin(__my_shader_u * MARBLE_PERIOD_Y * 0.5 + __my_shader_v * MARBLE_PERIOD_X * 0.9 + turbulence));

	//attenuate and combine colors
	__my_shader_Ci[0] = 0.7 * sine + 0.2 * sine2;
	__my_shader_Ci[1] = 0.7 * sine + 0.2 * sine2;
	__my_shader_Ci[2] = 0.7 * sine + 0.3 * sine2;

	__my_shader_N[0] = 0;
	__my_shader_N[1] = 1;
	__my_shader_N[2] = 0;

}

float WOOD_PERIOD = 10;
float WOOD_TURBULENCE = 6;
void WOOD() {
	//define a center
	float centerx = 0.2;
	float centery = 0.8;
	//distance to center
	float distx = __my_shader_u - centerx;
	float disty = __my_shader_v - centery;
	float dist = sqrt(distx*distx + disty*disty);
	//put into sin along with turbulence
	float turbulence = WOOD_TURBULENCE * noise.octaveNoise(__my_shader_u, __my_shader_v, 0, 1, 3);
	//sqrt to decrease width of dark rings
	float sine = sqrt(abs(sin(dist * WOOD_PERIOD * M_PI + turbulence)));
	//sine = (sine > 0.8) ? 1 : sine;

	//add some overall grain
	sine += 0.5 * noise.octaveNoise(__my_shader_u, __my_shader_v, 5, 4, 9);

	//attenuate and combine colors
	RtColor dark = { 0.45, 0.32, 0.22 };
	RtColor light = { 1, 0.83, 0.61 };

	__my_shader_Ci[0] = dark[0] + sine * (light[0] - dark[0]);
	__my_shader_Ci[1] = dark[1] + sine  * (light[1] - dark[1]);
	__my_shader_Ci[2] = dark[2] + sine  * (light[2] - dark[2]);
}

//Phong lighting
float LIGHTING_REFLECTIVITY = 32;
float LIGHTING_SPEC_ATTEN = 1.0f;
void LIGHTING() {
	//All positions are in eye space
	Vector4 lightPos(-2, 10, 0);
	Vector4 fragPos(__my_shader_P[0], __my_shader_P[1], __my_shader_P[2]);
	Vector4 normal = Vector4(__my_shader_N[0], __my_shader_N[1], __my_shader_N[2]).normalize();
	Vector4 eyeDir = -fragPos.normalize();

	//make normal frontfacing
	if (eyeDir.dot3(normal) < 0)
		normal = -normal;
	Vector4 lightDir = (lightPos - fragPos).normalize();

	//diffuse lighting factor
	float diffuse = max(lightDir.dot3(normal), 0.0f);

	//specular lighting factor
	Vector4 reflectDir = (-lightDir).reflect(normal);
	float specular = pow(max(eyeDir.dot3(reflectDir), 0.0f), LIGHTING_REFLECTIVITY) * LIGHTING_SPEC_ATTEN;

	__my_shader_Ci[0] = min((diffuse + 0.2f)*__my_shader_Ci[0] + specular*1.0f, 1.0f);
	__my_shader_Ci[1] = min((diffuse + 0.2f)*__my_shader_Ci[1] + specular*1.0f, 1.0f);
	__my_shader_Ci[2] = min((diffuse + 0.2f)*__my_shader_Ci[2] + specular*1.0f, 1.0f);
}

// Checkerboard
float CHECK_SIZE_X = 10.0f;
float CHECK_SIZE_Y = 10.0f;
void CHECKERBOARD() {
	if ((int)(floor(__my_shader_u*CHECK_SIZE_X) + floor(__my_shader_v*CHECK_SIZE_Y)) % 2 == 0) {
		__my_shader_Ci[0] = 1;
		__my_shader_Ci[1] = 1;
		__my_shader_Ci[2] = 1;
	}
	else {
		__my_shader_Ci[0] = 0;
		__my_shader_Ci[1] = 0;
		__my_shader_Ci[2] = 0;
	}

}

// Bumpy surface
float BUMP_AMPLITUDE = 1;
float BUMP_MIN_FREQ_EXP = 1;
float BUMP_MAX_FREQ_EXP = 2;
void BUMPY() {
	Vector4 normal(__my_shader_N[0], __my_shader_N[1], __my_shader_N[2]);
	Vector4 position(__my_shader_P[0], __my_shader_P[1], __my_shader_P[2]);
	
	//octaving
	float displacement = BUMP_AMPLITUDE * noise.octaveNoise(__my_shader_u, __my_shader_v, 0, BUMP_MIN_FREQ_EXP, BUMP_MAX_FREQ_EXP);

	//displace along normal
	position = position + normal.normalize() * displacement;

	__my_shader_P[0] = position.x;
	__my_shader_P[1] = position.y;
	__my_shader_P[2] = position.z;
}

void DONUT_DISPLACEMENT() {
	float donutSine = sin((__my_shader_v + 0.5f) * M_PI * 2);
	//barrier between dough and glaze
	float barrier = donutSine - (noise.octaveNoise(__my_shader_u, __my_shader_v, 1, 3, 6) + 0.5f);
	if (barrier > 0) {
		//make glaze stick out slightly
		Vector4 normal(__my_shader_N[0], __my_shader_N[1], __my_shader_N[2]);
		Vector4 position(__my_shader_P[0], __my_shader_P[1], __my_shader_P[2]);

		float displacement = min(0.05f, barrier * 0.05f);

		//displace along normal
		position = position + normal.normalize() * displacement;

		__my_shader_P[0] = position.x;
		__my_shader_P[1] = position.y;
		__my_shader_P[2] = position.z;
	}

	//make bumpy overall
	BUMP_AMPLITUDE = 0.05;
	BUMP_MIN_FREQ_EXP = 2;
	BUMP_MAX_FREQ_EXP = 9;
	BUMPY();
}

void DONUT_SURFACE() {
	//create the dough color which has a lighter band around the middle
	RtColor top = { 207.0f / 255, 115.0f / 255, 64.0f / 255 };
	RtColor middle = { 206.0f / 255, 185.0f / 255, 157.0f / 255 };
	float donutSine = sin((__my_shader_v + 0.5f) * M_PI * 2);
	float doughScale = pow(abs(sin(donutSine)), 0.6);

	mix(middle, top, doughScale, __my_shader_Ci);

	//create glaze on top
	RtColor glaze = { 93.0f/255, 52.0f/255, 34.0f/255 };
	//barrier between dough and glaze
	if (donutSine > noise.octaveNoise(__my_shader_u, __my_shader_v, 1, 3, 6) + 0.5f) {
		mix(__my_shader_Ci, glaze, 1.0f, __my_shader_Ci);
	}
}