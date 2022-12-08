#pragma once
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class SpotLight : public Light
{
	//Spotlight members
public:
	float constant = 1.0f;
	float linear = 0.5f;
	float quadratic = 0.1f;
	glm::vec3 direction;
	SpotLight(float xPos, float yPos, float zPos)
	{
		lightColor = glm::vec3(1, 1, 1);
		lightPosition = glm::vec3(xPos, yPos, zPos);
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		ambientStr = 1.0f;
		specPhog = 10.0f;
		specStr = 5.0f;
	}

	void setSpotLight(GLuint shaderProgram)
	{
		//Get Uniforms from Shader
		unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
		glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPosition));

		unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

		unsigned int ambientStrLoc = glGetUniformLocation(shaderProgram, "ambientStr");
		glUniform1f(ambientStrLoc, ambientStr);

		unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor");
		glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

		unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr");
		glUniform1f(specStrLoc, specStr);

		unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhog");
		glUniform1f(specPhongLoc, specPhog);

		unsigned int constantLoc = glGetUniformLocation(shaderProgram, "constant");
		glUniform1f(constantLoc, constant);

		unsigned int linLoc = glGetUniformLocation(shaderProgram, "lin");
		glUniform1f(linLoc, linear);

		unsigned int quadLoc = glGetUniformLocation(shaderProgram, "quad");
		glUniform1f(quadLoc, quadratic);

		unsigned int spotDirLoc = glGetUniformLocation(shaderProgram, "direction2");
		glUniform3f(spotDirLoc, direction.x, direction.y, direction.z);

		unsigned int spotCutOffLoc = glGetUniformLocation(shaderProgram, "cutOff");
		glUniform1f(spotCutOffLoc, glm::cos(glm::radians(8.5f)));
		unsigned int spotOutCutOffLoc = glGetUniformLocation(shaderProgram, "outerCutOff");
		glUniform1f(spotOutCutOffLoc, glm::cos(glm::radians(13.5f)));

	}


};
