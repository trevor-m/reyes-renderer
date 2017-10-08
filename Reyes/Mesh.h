#pragma once
#include "Ri.h"
#include "Shader.h"
#include "Matrix4.h"
#include "Vector4.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
using std::min;
using std::max;

// Shader function pointers
extern void(*Ri_shaderSurfaceFunction)();
extern void(*Ri_shaderDisplacementFunction)();

// A point in the grid at (u,v)
struct MeshPoint {
	bool valid;
	Vector4 position;
	Vector4 normal;
	float color[3];
	float opacity[3];
};

// A grid of points (u, v) with their corresponding positions and attributes
class Mesh {
public:
	int width, height;
	MeshPoint* grid;

	Mesh() {
		width = height = 0;
		grid = NULL;
	}

	Mesh(int width, int height) {
		this->width = width;
		this->height = height;
		grid = new MeshPoint[width * height];
	}

	~Mesh() {
		if (grid != NULL) {
			delete[] grid;
		}
	}

	MeshPoint *getGridPtr(int u, int v) {
		return (grid + (v * width + u));
	}

	// Apply MVP matrix to all points in grid
	void ApplyMatrix(Matrix4& matrix) {
		//calculate transposed inverse matrix for normals
		Matrix4 normalMatrix = matrix.inverse().transpose();
		//truncate into 3x3 to ignore translation component
		normalMatrix.m[3] = 0.0f;
		normalMatrix.m[7] = 0.0f;
		normalMatrix.m[11] = 0.0f;
		normalMatrix.m[12] = 0.0f;
		normalMatrix.m[13] = 0.0f;
		normalMatrix.m[14] = 0.0f;
		normalMatrix.m[15] = 0.0f;
		for (int u = 0; u < width; u++) {
			for (int v = 0; v < height; v++) {
				getGridPtr(u, v)->position = matrix * getGridPtr(u, v)->position;
				getGridPtr(u, v)->normal = (normalMatrix * getGridPtr(u, v)->normal).normalize();
			}
		}
	}

	// "Clip", perform perspective divide, and transform to pixel space for all points in the grid
	// Currently only used for initial bounding, as projection is now done during drawing phase(FrameBuffer::DrawMesh) so we can apply motion blur
	void Project(int frameWidth, int frameHeight) {
		//convert points to screen space
		for (int u = 0; u < width; u++) {
			for (int v = 0; v < height; v++) {
				//Clip against near and far
				if (getGridPtr(u, v)->position.z < 0 || getGridPtr(u, v)->position.z > getGridPtr(u, v)->position.w) {
					getGridPtr(u, v)->valid = false;
					//printf("invalid: ");
					//getGridPtr(u, v)->position.print();
					continue;
				}
				//TODO: clip against other planes
				getGridPtr(u, v)->valid = true;

				//perspective divide
				Vector4 pos = getGridPtr(u, v)->position.homogenize();

				//convert to pixel space
				getGridPtr(u, v)->position.x = (pos.x + 1.0f) * 0.5f * frameWidth;
				getGridPtr(u, v)->position.y = (-pos.y + 1.0f) * 0.5f * frameHeight; //y axis is inverted in pixel space
				getGridPtr(u, v)->position.z = pos.z;
				getGridPtr(u, v)->position.w = 1.0f;
			}
		}
	}

	// Apply displacement shader if one is being used
	void ShadeDisplacement() {
		if (Ri_shaderDisplacementFunction == NULL)
			return;

		//set input variables that don't change
		__my_shader_du = 1.0f / (width - 1);
		__my_shader_dv = 1.0f / (height - 1);

		for (int u = 0; u < width; u++) {
			for (int v = 0; v < height; v++) {
				//set input variables
				setShaderArguments(u, v, false);

				//run shader		
				Ri_shaderDisplacementFunction();

				//save modified values
				//position
				getGridPtr(u, v)->position.x = __my_shader_P[0];
				getGridPtr(u, v)->position.y = __my_shader_P[1];
				getGridPtr(u, v)->position.z = __my_shader_P[2];

				//normals 
				//normals will be recalculated
				/*
				getGridPtr(u, v)->normal.x = __my_shader_N[0];
				getGridPtr(u, v)->normal.y = __my_shader_N[1];
				getGridPtr(u, v)->normal.z = __my_shader_N[2];*/
			}
		}

		//recalculate normals
		for (int u = 0; u < width; u++) {
			for (int v = 0; v < height; v++) {
				int currU = (u == 0) ? u : u - 1;
				int currV = (v == 0) ? v : v - 1;
				int nextU = (u == width - 1) ? u : u + 1;
				int nextV = (v == height - 1) ? v : v + 1;
				Vector4 dPdu = (getGridPtr(nextU, v)->position - getGridPtr(currU, v)->position).normalize();
				Vector4 dPdv = (getGridPtr(u, nextV)->position - getGridPtr(u, currV)->position).normalize();
				getGridPtr(u, v)->normal = (dPdv.cross(dPdu)).normalize();
			}
		}
	}

	// Apply surface shader if one is being used or just do lighting
	void ShadeSurface() {
		//if (Ri_shaderSurfaceFunction == NULL)
		//	return;

		//set input variables that don't change
		__my_shader_du = 1.0f / (width - 1);
		__my_shader_dv = 1.0f / (height - 1);

		for (int u = 0; u < width; u++) {
			for (int v = 0; v < height; v++) {
				//set input variables
				setShaderArguments(u, v, true);

				//run shader		
				if (Ri_shaderSurfaceFunction != NULL)
					Ri_shaderSurfaceFunction();

				//always do lighting
				LIGHTING();

				//save modified values
				//color
				getGridPtr(u, v)->color[0] = __my_shader_Ci[0];
				getGridPtr(u, v)->color[1] = __my_shader_Ci[1];
				getGridPtr(u, v)->color[2] = __my_shader_Ci[2];
				//opacity
				getGridPtr(u, v)->opacity[0] = __my_shader_Oi[0];
				getGridPtr(u, v)->opacity[1] = __my_shader_Oi[1];
				getGridPtr(u, v)->opacity[2] = __my_shader_Oi[2];
			}
		}
	}

	void setShaderArguments(int u, int v, bool surface) {
		//position
		__my_shader_P[0] = getGridPtr(u, v)->position.x;
		__my_shader_P[1] = getGridPtr(u, v)->position.y;
		__my_shader_P[2] = getGridPtr(u, v)->position.z;
		//normal
		__my_shader_N[0] = getGridPtr(u, v)->normal.x;
		__my_shader_N[1] = getGridPtr(u, v)->normal.y;
		__my_shader_N[2] = getGridPtr(u, v)->normal.z;
		//color
		__my_shader_Ci[0] = getGridPtr(u, v)->color[0];
		__my_shader_Ci[1] = getGridPtr(u, v)->color[1];
		__my_shader_Ci[2] = getGridPtr(u, v)->color[2];
		//opacity
		__my_shader_Oi[0] = getGridPtr(u, v)->opacity[0];
		__my_shader_Oi[1] = getGridPtr(u, v)->opacity[1];
		__my_shader_Oi[2] = getGridPtr(u, v)->opacity[2];
		//u,v
		__my_shader_u = (float)u / (width - 1);
		__my_shader_v = (float)v / (height - 1);
		//derivatives
		//use prev and next point, except on edges of mesh
		int currU = (u == 0) ? u : u - 1;
		int currV = (v == 0) ? v : v - 1;
		int nextU = (u == width - 1) ? u : u + 1;
		int nextV = (v == height - 1) ? v : v + 1;
		Vector4 dPdu = (getGridPtr(nextU, v)->position - getGridPtr(currU, v)->position) * (1.0f/__my_shader_du);
		__my_shader_dPdu[0] = dPdu.x;
		__my_shader_dPdu[1] = dPdu.y;
		__my_shader_dPdu[2] = dPdu.z;
		Vector4 dPdv = (getGridPtr(u, nextV)->position - getGridPtr(u, currV)->position) * (1.0f / __my_shader_dv);
		__my_shader_dPdv[0] = dPdv.x;
		__my_shader_dPdv[1] = dPdv.y;
		__my_shader_dPdv[2] = dPdv.z;
	}
};