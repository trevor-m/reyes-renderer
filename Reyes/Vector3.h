#pragma once
#define VECTOR3_SIZE 3

class Vector3 {
public:
	//data
	float v[VECTOR3_SIZE];

	//Initialize to (0,0,0)
	Vector3();
	//Initialize to (x,y,1)
	Vector3(float x, float y);
	//Initialize to (x,y,z)
	Vector3(float x, float y, float z);

	//Inverse
	Vector3 operator-();

	//Addition
	Vector3 operator+(const Vector3& other);

	//Subtraction
	Vector3 operator-(const Vector3& other);

	//Multiplication
	Vector3 operator*(float scalar);
	float dot(const Vector3& other);
	Vector3 cross(const Vector3& other);

	//Accessors
	float x();
	float y();
	float z();
};

