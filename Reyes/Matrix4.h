#pragma once
#include "Vector4.h"
#define _USE_MATH_DEFINES
#include <cmath>

#define MATRIX_SIZE 4
#define MATRIX_NUM_ELEM 16

// 4x4 Matrix
class Matrix4 {
public:
	//Data arranged in column-major order:
	// 0  4  8  12
	// 1  5  9  13
	// 2  6  10 14
	// 3  7  11 15
	float m[MATRIX_NUM_ELEM];
	
	//Create a zero matrix
	Matrix4();
	~Matrix4();

	//Create a matrix from a 4x4 array
	Matrix4(float values[4][4]);

	//Create an identity matrix
	static Matrix4 Identity();

	//Create a 3D translation matrix
	static Matrix4 Translate(float x, float y, float z);

	//Create a scaling matrix
	static Matrix4 Scale(float x, float y, float z);

	//Create a rotation matrix around x axis
	static Matrix4 RotateX(float angle);
	//Create a rotation matrix around y axis
	static Matrix4 RotateY(float angle);
	//Create a rotation matrix around z axis
	static Matrix4 RotateZ(float angle);
	//Create a rotation matrix around arbitrary axis x, y, z
	static Matrix4 Rotate(float angle, float x, float y, float z);

	//Create an orthographic projection matrix
	static Matrix4 Orthographic(float left, float right, float top, float bottom, float near, float far);
	//Create a perspective projection matrix where near z is mapped to 0 and far z is mapped to 1
	static Matrix4 Perspective(float fov, float aspect, float near, float far);

	//Addition
	//Matrix4 operator+(const Matrix4& other);

	//Subtraction
	//Matrix4 operator-(const Matrix4& other);

	//Multiplication
	Matrix4 operator*(const Matrix4& other);
	Vector4 operator*(const Vector4& other);
	Matrix4 operator*(float scalar);

	//Transpose
	Matrix4 transpose();

	//Inverse
	Matrix4 inverse();

	//Accessors
	//float get(int row, int column);
};

