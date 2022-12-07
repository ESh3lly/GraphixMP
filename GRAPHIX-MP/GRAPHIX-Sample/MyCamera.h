#pragma once
#include <iostream>

float radius = 0.1f;
float camX = 0.0f;
float camZ = 1.f;
float width = 750.0f;
float height = 750.0f;

class MyCamera {
public:
	float leftPoint;
	float rightPoint;
	float botPoint;
	float topPoint;
	float zNear;
	float zFar;
	char wala;

	/*Parent class for PErspective Camera and Orthographic Projection*/
	MyCamera() {
		camX = 0;
	}

	MyCamera(int n) {
		leftPoint = n;
	};

	MyCamera(float rp, float lp, float bp, float tp, float zn, float zf) {
		leftPoint = lp;
		rightPoint = rp;
		botPoint = bp;
		topPoint = tp;
		zNear = zn;
		zFar = zf;
	};


};