#pragma once
#include "Vector4.h"

// A triangle. Used only for sampling micropolygons
class Triangle {
public:
	Vector4 v[3];

	Triangle() {

	}

	Triangle(Vector4& v0, Vector4& v1, Vector4& v2) {
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}

	Vector4 normal() {
		Vector4 v0v1 = v[1] - v[0];
		Vector4 v0v2 = v[2] - v[0];
		return v0v1.cross(v0v2);
	}

	bool shadowTest(Vector4& line) {
		return (line.x == 0) ? (line.y > 0) : (line.y > 0);
	}

	bool insideLine(float e, Vector4& line) {
		return (e == 0) ? shadowTest(line) : (e > 0);
	}

	bool insideTriangle(float x, float y) {
		Vector4 line0 = v[1].cross2(v[0]);
		Vector4 line1 = v[2].cross2(v[1]);
		Vector4 line2 = v[0].cross2(v[2]);

		//invert lines if vertices are not in CCW order
		Vector4 v0v1 = v[1] - v[0];
		Vector4 v0v2 = v[2] - v[0];
		Vector4 normal = v0v1.cross(v0v2);
		bool ccw = (normal.z < 0);
		if (!ccw) {
			line0 = -line0;
			line1 = -line1;
			line2 = -line2;
		}
		//TODO add shadow test
		Vector4 point(x, y, 1);
		if (insideLine(point.dot3(line0), line0) && insideLine(point.dot3(line1), line1) && insideLine(point.dot3(line2), line2))
			return true;
		return false;
	}
};