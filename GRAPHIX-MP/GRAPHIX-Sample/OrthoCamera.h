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
		
		glm::vec3 OrthoCamPos = glm::vec3(0, 10.f, 0.f);
		glm::vec3 OrthoUp = glm::vec3(0, 1.0f, -1.f);
		glm::vec3 OrthoCamCenter = glm::vec3(0, 6.f, 1.f);

		glm::mat4 view_matrix = glm::lookAt(OrthoCamPos, OrthoCamPos + OrthoCamCenter, OrthoUp);

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