#include "Vector4.h"



Vector4::Vector4() {
	x = y = z = w = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1;
}

Vector4 Vector4::operator+(const Vector4& other) {
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other) {
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*(float scalar) {
	return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4 Vector4::cross2(const Vector4& other) {
	// x    y    z
	//v[x] v[y] v[z]
	//o[x] o[y] o[z]
	Vector4  result;
	result.x = y * 1 - 1 * other.y;
	result.y = -x * 1 + 1 * other.x;
	result.z = x * other.y - y * other.x;
	return result;
}

Vector4 Vector4::cross(const Vector4& other) {
	// x    y    z
	//v[x] v[y] v[z]
	//o[x] o[y] o[z]
	Vector4  result;
	result.x = y * other.z - z * other.y;
	result.y = -x * other.z + z * other.x;
	result.z = x * other.y - y * other.x;
	return result;
}