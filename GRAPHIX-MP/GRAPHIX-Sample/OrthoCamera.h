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
		glDisable(GL_BLEND);

		
		float ccx = cos(glm::radians(orthoYaw)) * cos(glm::radians(orthoPitch));
		float ccy = sin(glm::radians(orthoPitch));
		float ccz = sin(glm::radians(orthoYaw)) * cos(glm::radians(orthoPitch));

		glm::vec3 OrthoCamPos = glm::vec3(subPos_x, 10.f, subPos_z);
		glm::vec3 OrthoUp = glm::vec3(0, 1.0f, 0.f);
		glm::vec3 OrthoCamCenter = glm::normalize(glm::vec3(ccx, ccy, ccz));

		glm::mat4 view_matrix = glm::lookAt(OrthoCamPos, OrthoCamPos + OrthoCamCenter, OrthoUp);

		glm::mat4 projection_matrix = glm::ortho(leftPoint, rightPoint, botPoint, topPoint, zNear, zFar);

		unsigned int cameraPosLoc = glGetUniformLocation(shaderProg, "cameraPos");
		glUniform3fv(cameraPosLoc, 1, glm::value_ptr(OrthoCamPos));

		unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
		glUniformMatrix4fv(viewLoc,	1,	GL_FALSE, glm::value_ptr(view_matrix));

		unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
			
	}

};