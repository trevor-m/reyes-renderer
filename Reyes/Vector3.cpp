#include "Vector3.h"



Vector3::Vector3() {
	for (int i = 0; i < VECTOR3_SIZE; i++)
		v[i] = 0.0f;
}

Vector3::Vector3(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

Vector3::Vector3(float x, float y) {
	v[0] = x;
	v[1] = y;
	v[2] = 1.0f;
}

Vector3 Vector3::operator-() {
	return Vector3(-v[0], -v[1], -v[2]);
}

Vector3 Vector3::operator+(const Vector3& other) {
	return Vector3(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
}

Vector3 Vector3::operator-(const Vector3& other) {
	return Vector3(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
}

Vector3 Vector3::operator*(float scalar) {
	return Vector3(v[0] * scalar, v[1] * scalar, v[2] * scalar);
}

float Vector3::dot(const Vector3& other) {
	return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}

Vector3 Vector3::cross(const Vector3& other) {
	// x    y    z
	//v[0] v[1] v[2]
	//o[0] o[1] o[2]
	Vector3 result;
	result.v[0] = v[1] * other.v[2] - v[2] * other.v[1];
	result.v[1] = -v[0] * other.v[2] + v[2] * other.v[0];
	result.v[2] = v[0] * other.v[1] - v[1] * other.v[0];
	return result;
}

float Vector3::x() {
	return v[0];
}

float Vector3::y() {
	return v[1];
}

float Vector3::z() {
	return v[2];
}