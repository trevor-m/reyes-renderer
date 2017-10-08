#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#define VECTOR4_SIZE 4

//4-D Vector
class Vector4 {
public:
	//data
	float x, y, z, w;

	//Initialize to (0,0,0,0)
	Vector4();
	//Initialize to (x,y,z,w)
	Vector4(float x, float y, float z, float w);
	//Initialize to (x,y,z,1)
	Vector4(float x, float y, float z);

	//Addition
	Vector4 operator+(const Vector4& other);

	//Subtraction
	Vector4 operator-(const Vector4& other);

	//Multiplication
	Vector4 operator*(float scalar);

	//2D Cross Product (Mainly for rasterization)
	Vector4 cross2(const Vector4& other);

	//3D Cross Product
	Vector4 cross(const Vector4& other);

	Vector4 homogenize() {
		Vector4 result;
		result.x = this->x / this->w;
		result.y = this->y / this->w;
		result.z = this->z / this->w;
		result.w = 1.0f;
		return result;
	}

	float dot3(Vector4& other) {
		return x * other.x + y * other.y + z * other.z;
	}

	Vector4 reflect(Vector4& normal) {
		return *this - normal * 2 * this->dot3(normal);
	}

	Vector4 operator -() {
		return Vector4(-x, -y, -z, -w);
	}

	float length() {
		return sqrt(x*x + y*y + z*z);
	}

	Vector4 normalize() {
		float magnitude = length();
		return Vector4(x / magnitude, y / magnitude, z / magnitude);
	}

	void print() {
		printf("[%f %f %f %f]\n", x, y, z, w);
	}
};

