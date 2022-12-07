#pragma once
#pragma once
#pragma once
#include "MyCamera.h"
class OrthoCamera : public MyCamera {
public:
	float leftPoint;
	float rightPoint;
	float botPoint;
	float topPoint;
	float zNear;
	float zFar;

	/*initialize orthographic projection by space coordinates
	overloads MyCamera constructor*/
	OrthoCamera(float rp, float lp, float bp, float tp, float zn, float zf) {
		leftPoint = lp;
		rightPoint = rp;
		botPoint = bp;
		topPoint = tp;
		zNear = zn;
		zFar = zf;
	};

	/*orthographic projection draw part*/
	void getOCamera(GLuint shaderProg) {

		glm::mat4 view_matrix = glm::lookAt(cameraPos, cameraPos + CameraCenter, WorldUp);

		glm::mat4 projection_matrix = glm::ortho(leftPoint, rightPoint, botPoint, topPoint, zNear, zFar);

		unsigned int cameraPosLoc = glGetUniformLocation(shaderProg, "cameraPos");
		glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

		unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
		glUniformMatrix4fv(
			viewLoc,  //address of transform variable
			1,  //matrices to assign
			GL_FALSE,   //Transpose?
			glm::value_ptr(view_matrix)
		);

		unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
		glUniformMatrix4fv(
			projectionLoc,  //address of transform variable
			1,  //matrices to assign
			GL_FALSE,   //Transpose?
			glm::value_ptr(projection_matrix)
		);
	}

};