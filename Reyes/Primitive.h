#pragma once
#include "Matrix4.h"
#include "Mesh.h"

#define MICROPOLYGONS_PER_PIXEL 2

//which solid color new primitives are set to
extern RtColor Ri_drawColor;
//which opacity new primtives are set to
extern RtColor Ri_drawOpacity;

// A primitive that can be diced into a Mesh
class Primitive {
protected:
	virtual void getPositionAndNormal(float u, float v, Vector4& position, Vector4& normal) = 0;
	virtual void getBounds(Vector4& bbMin, Vector4& bbMax) = 0;
public:
	bool Bound(Matrix4& mvp, int frameWidth, int frameHeight, int& diceFactor) {
		//get a feel for how big the primitive will be
		//TODO: account for possible displacement
		//diceFactor = 100;
		//return true;
		Mesh* coarseMesh = Dice(8, 8);
		coarseMesh->ApplyMatrix(mvp);
		coarseMesh->Project(frameWidth, frameHeight);
		//get bounds from mesh
		float x_min = FLT_MAX;
		float x_max = FLT_MIN;
		float y_min = FLT_MAX;
		float y_max = FLT_MIN;
		bool valid = false;
		for (int u = 0; u < coarseMesh->width; u++) {
			for (int v = 0; v < coarseMesh->width; v++) {
				MeshPoint* point = coarseMesh->getGridPtr(u, v);
				//only look at valid points
				if (!point->valid)
					continue;
				//must have one valid point
				valid = true;
				if (point->position.x < x_min)
					x_min = point->position.x;
				if (point->position.x > x_max)
					x_max = point->position.x;
				if (point->position.y < y_min)
					y_min = point->position.y;
				if (point->position.y > y_max)
					y_max = point->position.y;
			}
		}
		delete coarseMesh;
		if (!valid)
			return false;

		//bind to screen dimensions
		//y_max = min((int)y_max, frameHeight);
		//y_min = max((int)y_min, 0);
		//x_max = min((int)x_max, frameWidth);
		//x_min = max((int)x_min, 0);

		//if it will be on the screen, how many pixels will it take up?
		int xFactor = (x_max - x_min) * MICROPOLYGONS_PER_PIXEL;
		int yFactor = (y_max - y_min) * MICROPOLYGONS_PER_PIXEL;

		//choose the largest dimension
		diceFactor = max(xFactor, yFactor);
		//can't be higher than screen resolution*MICROPOLYGONS_PER_PIXEL
		diceFactor = min(diceFactor, max(frameWidth, frameHeight) * MICROPOLYGONS_PER_PIXEL * 2);

		return true;
	}

	Mesh* Dice(int xFactor, int yFactor) {
		Mesh* mesh = new Mesh(xFactor, yFactor);
		for (int u = 0; u < mesh->width; u++) {
			for (int v = 0; v < mesh->height; v++) {
				//normalized u, v which runs from 0 to 1
				float u_n = (float)u / (mesh->width - 1);
				float v_n = (float)v / (mesh->height - 1);

				//get position and normal through overridden methods per each type of primitive
				getPositionAndNormal(u_n, v_n, mesh->getGridPtr(u, v)->position, mesh->getGridPtr(u, v)->normal);

				//set to draw color(set by RiColor) to begin
				mesh->getGridPtr(u, v)->color[0] = Ri_drawColor[0];
				mesh->getGridPtr(u, v)->color[1] = Ri_drawColor[1];
				mesh->getGridPtr(u, v)->color[2] = Ri_drawColor[2];
				//set opacity
				mesh->getGridPtr(u, v)->opacity[0] = Ri_drawOpacity[0];
				mesh->getGridPtr(u, v)->opacity[1] = Ri_drawOpacity[1];
				mesh->getGridPtr(u, v)->opacity[2] = Ri_drawOpacity[2];
			}
		}
		return mesh;
	}
};

class Sphere : public Primitive {
protected:
	//intial parameters
	float radius;
	//calculated parameters for dicing
	float phiMin, phiMax, thetaMax;

	virtual void getPositionAndNormal(float u, float v, Vector4& position, Vector4& normal) {
		float phi = phiMin + v * (phiMax - phiMin);
		float theta = u * thetaMax;

		position.x = radius * cos(theta) * cos(phi);
		position.y = radius * sin(theta) * cos(phi);
		position.z = radius * sin(phi);
		position.w = 1.0f;

		normal = position.normalize();
	}

	virtual void getBounds(Vector4& bbMin, Vector4& bbMax) {
		bbMin = Vector4(-radius, -radius, -radius);
		bbMax = Vector4(radius, radius, radius);
	}

public:
	Sphere(float radius, float zmin, float zmax, float tmax) {
		this->radius = radius;

		//calculate necessary parameters (in radians)
		this->phiMin = (zmin > -radius) ? asin(zmin / radius) : -M_PI_2;
		this->phiMax = (zmax < radius) ? asin(zmax / radius) : M_PI_2;
		this->thetaMax = tmax * M_PI / 180.0f;
	}
};

class Cone : public Primitive {
protected:
	//intial parameters
	float height, radius;
	//calculated parameters for dicing
	float thetaMax;

	virtual void getPositionAndNormal(float u, float v, Vector4& position, Vector4& normal) {
		float theta = u * thetaMax;

		position.x = radius * (1 - v) * cos(theta);
		position.y = radius * (1 - v) * sin(theta);
		position.z = v * height;
		position.w = 1.0f;

		//vector from center to position
		Vector4 V = Vector4(position.x, position.y, 0).normalize();
		normal.x = V.x * height / radius;
		normal.y = V.y * height / radius;
		normal.z = radius / height;
		normal.w = 1.0f;
		normal = normal.normalize();
	}

	virtual void getBounds(Vector4& bbMin, Vector4& bbMax) {
		bbMin = Vector4(-radius, -radius, 0);
		bbMax = Vector4(radius, radius, height);
	}

public:
	Cone(float height, float radius, float tmax) {
		this->radius = radius;
		this->height = height;

		//calculate necessary parameters (in radians)
		this->thetaMax = tmax * M_PI / 180.0f;
	}
};

class Cylinder : public Primitive {
protected:
	//intial parameters
	float radius, zmin, zmax;
	//calculated parameters for dicing
	float thetaMax;

	virtual void getPositionAndNormal(float u, float v, Vector4& position, Vector4& normal) {
		float theta = u * thetaMax;

		position.x = radius * cos(theta);
		position.y = radius * sin(theta);
		position.z = zmin + v * (zmax - zmin);
		position.w = 1.0f;

		normal = Vector4(position.x, position.y, 0).normalize();
	}

	virtual void getBounds(Vector4& bbMin, Vector4& bbMax) {
		bbMin = Vector4(-radius, -radius, zmin);
		bbMax = Vector4(radius, radius, zmax);
	}

public:
	Cylinder(float radius, float zmin, float zmax, float tmax) {
		this->radius = radius;
		this->zmax = zmax;
		this->zmin = zmin;

		//calculate necessary parameters (in radians)
		this->thetaMax = tmax * M_PI / 180.0f;
	}
};

class Torus : public Primitive {
protected:
	//intial parameters
	float majorradius, minorradius;
	//calculated parameters for dicing
	float phiMin, phiMax, thetaMax;

	virtual void getPositionAndNormal(float u, float v, Vector4& position, Vector4& normal) {
		float phi = phiMin + v * (phiMax - phiMin);
		float theta = u * thetaMax;
		float r = minorradius * cos(phi);

		position.x = (majorradius + r) * cos(theta);
		position.y = (majorradius + r) * sin(theta);
		position.z = minorradius * sin(phi);
		position.w = 1.0f;

		normal = (position - Vector4(majorradius*cos(theta), majorradius*sin(theta), 0)).normalize();
	}

	virtual void getBounds(Vector4& bbMin, Vector4& bbMax) {
		bbMin = Vector4(-(majorradius+minorradius), -(majorradius + minorradius), -minorradius);
		bbMax = Vector4((majorradius + minorradius), (majorradius + minorradius), minorradius);
	}

public:
	Torus(float majorradius, float minorradius, float pmin, float pmax, float tmax) {
		this->majorradius = majorradius;
		this->minorradius = minorradius;

		//calculate necessary parameters (in radians)
		this->phiMin = pmin * M_PI / 180.0f;
		this->phiMax = pmax * M_PI / 180.0f;
		this->thetaMax = tmax * M_PI / 180.0f;
	}
};