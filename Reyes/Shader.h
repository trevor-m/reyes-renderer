#pragma once
#include "Ri.h"
// This file provides forward declarations of shaders and their "parameters"
// These shaders should be implemented in Shader.cpp
// To use a shader, call RiSurface(shaderFunction) or RiDisplacement(shaderFunction) before issuing a primitive

// Shader input/outputs
extern RtPoint __my_shader_P;
extern RtPoint __my_shader_N;
extern RtColor __my_shader_Ci;
extern RtColor __my_shader_Oi;
extern RtFloat __my_shader_u;
extern RtFloat __my_shader_v;
extern RtFloat __my_shader_du;
extern RtFloat __my_shader_dv;
extern RtVector __my_shader_dPdu;
extern RtVector __my_shader_dPdv;

//------------------------------------------------
// Textures
//------------------------------------------------

//Loads color into Ci from texture unit at u,v coords
extern float TEXTURE_TILE_X;
extern float TEXTURE_TILE_Y;
void texture(int unit);

//------------------------------------------------
// Surface shaders
//------------------------------------------------

// Phong lighting - used automatically on everything after surface shader is run
extern float LIGHTING_REFLECTIVITY;
extern float LIGHTING_SPEC_ATTEN;
void LIGHTING();

// Checkerboard pattern
extern float CHECK_SIZE_X;
extern float CHECK_SIZE_Y;
void CHECKERBOARD();

void MARBLE();

void WOOD();

void DONUT_SURFACE();

//------------------------------------------------
// Displacement shaders
//------------------------------------------------

// Bumpy surface
extern float BUMP_AMPLITUDE;
extern float BUMP_MIN_FREQ_EXP;
extern float BUMP_MAX_FREQ_EXP;
void BUMPY();

void DONUT_DISPLACEMENT();