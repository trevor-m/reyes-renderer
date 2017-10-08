#include "Matrix4.h"



Matrix4::Matrix4() {
	//zeros
	for (int i = 0; i < MATRIX_NUM_ELEM; i++) {
		m[i] = 0.0f;
	}
}

Matrix4::~Matrix4() {
}

Matrix4::Matrix4(float values[4][4]) {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			m[j*MATRIX_SIZE + i] = values[i][j];
		}
	}
}

Matrix4 Matrix4::Identity() {
	Matrix4 result;
	result.m[0] = 1.0f;
	result.m[5] = 1.0f;
	result.m[10] = 1.0f;
	result.m[15] = 1.0f;
	return result;
}

Matrix4 Matrix4::Translate(float x, float y, float z) {
	//start with identity
	Matrix4 result = Matrix4::Identity();
	result.m[12] = x;
	result.m[13] = y;
	result.m[14] = z;
	return result;
}

Matrix4 Matrix4::Scale(float x, float y, float z) {
	Matrix4 result;
	result.m[0] = x;
	result.m[5] = y;
	result.m[10] = z;
	result.m[15] = 1.0f;
	return result;
}

Matrix4 Matrix4::RotateX(float angle) {
	Matrix4 result;
	result.m[0] = 1.0f;
	result.m[5] = cos(angle);
	result.m[9] = -sin(angle);
	result.m[6] = sin(angle);
	result.m[10] = cos(angle);
	result.m[15] = 1.0f;
	return result;
}

Matrix4 Matrix4::RotateY(float angle) {
	Matrix4 result;
	result.m[0] = cos(angle);
	result.m[8] = sin(angle);
	result.m[5] = 1.0f;
	result.m[2] = -sin(angle);
	result.m[10] = cos(angle);
	result.m[15] = 1.0f;
	return result;
}

Matrix4 Matrix4::RotateZ(float angle) {
	Matrix4 result;
	result.m[0] = cos(angle);
	result.m[4] = -sin(angle);
	result.m[1] = sin(angle);
	result.m[5] = cos(angle);
	result.m[10] = 1.0f;
	result.m[15] = 1.0f;
	return result;
}

Matrix4 Matrix4::Rotate(float angle, float x, float y, float z) {
	Vector4 axis(x, y, z);
	axis = axis.normalize();
	x = axis.x;
	y = axis.y;
	z = axis.z;
	Matrix4 result;
	result.m[0] = x * x * (1 - cos(angle)) + cos(angle);
	result.m[1] = x * y * (1 - cos(angle)) + z * sin(angle);
	result.m[2] = x * z * (1 - cos(angle)) - y * sin(angle);
	result.m[4] = y * x * (1 - cos(angle)) - z * sin(angle);
	result.m[5] = y * y * (1 - cos(angle)) + cos(angle);
	result.m[6] = y * z * (1 - cos(angle)) + x * sin(angle);
	result.m[8] = z * x * (1 - cos(angle)) + y * sin(angle);
	result.m[9] = z * y * (1 - cos(angle)) - x * sin(angle);
	result.m[10] = z * z * (1 - cos(angle)) + cos(angle);
	result.m[15] = 1.0f;
	return result;
}

Matrix4 Matrix4::Orthographic(float left, float right, float top, float bottom, float near, float far) {
	Matrix4 result;
	result.m[0] = 2.0f / (right - left);
	result.m[5] = 2.0f / (top - bottom);
	result.m[10] = 1.0f / (far - near);
	result.m[15] = 1.0f;
	result.m[3] = -(right + left) / (right - left);
	result.m[7] = -(top + bottom) / (top - bottom);
	result.m[11] = 1 -(far + near) / (far - near);
	return result;
}

Matrix4 Matrix4::Perspective(float fov, float aspect, float near, float far) {
	float f = 1.0f / tan(fov / 2.0f);
	Matrix4 result;
	result.m[0] = f / aspect; //f
	result.m[5] = f;//1.0f / tan(fov / (aspect *2.0f)); //
	result.m[10] = far / (far - near); //(far + near) / (near - far);
	result.m[14] = -far * near / (far - near); //-2 * far*near / (near - far);
	result.m[11] = 1.0f;
	return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) {
	Matrix4 result;
	//result.m[0] = m[0] * other.m[0] + m[4] * other.m[1] + m[8] * other.m[2] + m[12] * other.m[3];
	for (int c = 0; c < MATRIX_SIZE; c++) {
		for (int r = 0; r < MATRIX_SIZE; r++) {
			result.m[r + c * 4] = 0.0f;
			for (int i = 0; i < MATRIX_SIZE; i++) {
				result.m[r + c * 4] += m[r + i * 4] * other.m[i + c * 4];
			}
		}
	}
	return result;
}

Vector4 Matrix4::operator*(const Vector4& other) {
	Vector4 result;
	result.x = other.x * m[0] + other.y * m[4] + other.z * m[8] + other.w * m[12];
	result.y = other.x * m[1] + other.y * m[5] + other.z * m[9] + other.w * m[13];
	result.z = other.x * m[2] + other.y * m[6] + other.z * m[10] + other.w * m[14];
	result.w = other.x * m[3] + other.y * m[7] + other.z * m[11] + other.w * m[15];
	/*for (int i = 0; i < MATRIX_SIZE; i++) {
	result.v[i] = 0.0f;
	for (int j = 0; j < VECTOR4_SIZE; j++) {
	result.v[i] += other.v[j] * m[i + j * 4];
	}
	}*/
	return result;
}

Matrix4 Matrix4::operator*(float scalar) {
	Matrix4 result;
	for (int i = 0; i < MATRIX_NUM_ELEM; i++) {
		result.m[i] = m[i] * scalar;
	}
	return result;
}

Matrix4 Matrix4::transpose() {
	Matrix4 result;
	result.m[0] = m[0];
	result.m[1] = m[4];
	result.m[2] = m[8];
	result.m[3] = m[12];
	result.m[4] = m[1];
	result.m[5] = m[5];
	result.m[6] = m[9];
	result.m[7] = m[13];
	result.m[8] = m[2];
	result.m[9] = m[6];
	result.m[10] = m[10];
	result.m[11] = m[14];
	result.m[12] = m[3];
	result.m[13] = m[7];
	result.m[14] = m[11];
	result.m[15] = m[15];
	return result;
}

Matrix4 Matrix4::inverse() {
	//
	// Inversion by Cramer's rule.  Code taken from an Intel publication
	// Source: https://graphics.stanford.edu/~mdfisher/Code/Engine/Matrix4.cpp.html
	// Original Source: http://download.intel.com/design/PentiumIII/sml/24504301.pdf
	//
	double Result[4][4];
	double tmp[12]; /* temp array for pairs */
	double src[16]; /* array of transpose source matrix */
	double det; /* determinant */
				/* transpose matrix */
	for (int i = 0; i < 4; i++)
	{
		src[i + 0] = m[i*4 + 0];
		src[i + 4] = m[i*4 + 1];
		src[i + 8] = m[i*4 + 2];
		src[i + 12] = m[i*4 + 3];
	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	/* calculate first 8 elements (cofactors) */
	Result[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	Result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	Result[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	Result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	Result[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	Result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	Result[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	Result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	Result[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	Result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	Result[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	Result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	Result[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	Result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	Result[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	Result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];

	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];
	/* calculate second 8 elements (cofactors) */
	Result[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	Result[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	Result[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	Result[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	Result[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	Result[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	Result[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	Result[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	Result[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	Result[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	Result[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	Result[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	Result[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	Result[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	Result[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	Result[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
	/* calculate determinant */
	det = src[0] * Result[0][0] + src[1] * Result[0][1] + src[2] * Result[0][2] + src[3] * Result[0][3];
	/* calculate matrix inverse */
	det = 1.0f / det;
	//End code

	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i*4 + j] = float(Result[i][j] * det);
		}
	}
	return result;
}