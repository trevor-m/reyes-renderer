#pragma once
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <algorithm>

#define GRID_SIZE 256

// 3-D Perlin Noise
class Noise {
private:
	// 3D cube of gradients (3-D vectors)
	float gradients[GRID_SIZE+1][GRID_SIZE+1][GRID_SIZE+1][3];

	float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}

	// fifth degree interpolator 6t^5 - 15t^4 + 10t^3
	// as used in Perlin's improved algorithm
	// http://http.developer.nvidia.com/GPUGems/gpugems_ch05.html
	float smooth(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	// dot product between distance vector of u,v,w and gradient at grid position x,y,z
	float gridGradient(int x, int y, int z, float u, float v, float w) {
		float du = u - x;
		float dv = v - y;
		float dw = w - z;
		return du * gradients[x][y][z][0] + dv * gradients[x][y][z][1] + dw * gradients[x][y][z][2];
	}

public:
	Noise() {
		//create grid
		for (int x = 0; x < GRID_SIZE; x++) {
			for (int y = 0; y < GRID_SIZE; y++) {
				for (int z = 0; z < GRID_SIZE; z++) {
					//pick a point on the unit sphere
					//using formulas 6, 7, 8 on http://mathworld.wolfram.com/SpherePointPicking.html
					float theta = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
					float u = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

					gradients[x][y][z][0] = sqrt(1 - u*u) * cos(theta);
					gradients[x][y][z][1] = sqrt(1 - u*u) * sin(theta);
					gradients[x][y][z][2] = u;
				}
			}
		}
		//duplicate first layer as last layer
		for (int i = 0; i < 3; i++) {
			gradients[GRID_SIZE - 1][GRID_SIZE-1][GRID_SIZE][i] = gradients[GRID_SIZE - 1][GRID_SIZE-1][0][i];
			gradients[GRID_SIZE - 1][GRID_SIZE][GRID_SIZE-1][i] = gradients[GRID_SIZE - 1][0][GRID_SIZE - 1][i];
			gradients[GRID_SIZE-1][GRID_SIZE][GRID_SIZE][i] = gradients[GRID_SIZE-1][0][0][i];
			gradients[GRID_SIZE][GRID_SIZE - 1][GRID_SIZE - 1][i] = gradients[0][GRID_SIZE - 1][GRID_SIZE - 1][i];
			gradients[GRID_SIZE][GRID_SIZE - 1][GRID_SIZE][i] = gradients[0][GRID_SIZE - 1][0][i];
			gradients[GRID_SIZE][GRID_SIZE][GRID_SIZE - 1][i] = gradients[0][0][GRID_SIZE - 1][i];
			gradients[GRID_SIZE][GRID_SIZE][GRID_SIZE][i] = gradients[0][0][0][i];
		}
	}

	float noise(float x, float y, float z) {
		//which cube to use (tile for points after GRID_SIZE or before 0)
		//min corner
		int x0 = (int)floor(x) % GRID_SIZE;
		int y0 = (int)floor(y) % GRID_SIZE;
		int z0 = (int)floor(z) % GRID_SIZE;
		//max corner
		int x1 = x0 + 1;
		int y1 = y0 + 1;
		int z1 = z0 + 1;

		//wrap around for point within cube
		x = x - floor(x) + x0;
		y = y - floor(y) + y0;
		z = z - floor(z) + z0;

		//get point within cube from 0 to 1 and smooth it
		//used as interpolant weights
		float dx = smooth(x - floor(x));
		float dy = smooth(y - floor(y));
		float dz = smooth(z - floor(z));

		//interpolate between corners of grid
		//x dimension
		float c00 = lerp(gridGradient(x0, y0, z0, x, y, z), gridGradient(x1, y0, z0, x, y, z), dx);
		float c01 = lerp(gridGradient(x0, y0, z1, x, y, z), gridGradient(x1, y0, z1, x, y, z), dx);
		float c10 = lerp(gridGradient(x0, y1, z0, x, y, z), gridGradient(x1, y1, z0, x, y, z), dx);
		float c11 = lerp(gridGradient(x0, y1, z1, x, y, z), gridGradient(x1, y1, z1, x, y, z), dx);
		//y dimension
		float c0 = lerp(c00, c10, dy);
		float c1 = lerp(c01, c11, dy);
		//z dimension
		return lerp(c0, c1, dz);
	}

	float octaveNoise(float x, float y, float z, int start, int stop) {
		float n = 0;
		float total = 0;
		for (int i = start; i <= stop; i++) {
			float amplitude = 1.0f / pow(2, i);
			total += amplitude;
			n += amplitude * noise(x * pow(2, i), y * pow(2, i), z);
			//amplitude *= 0.5f;
		}
		return n / total;
	}
};