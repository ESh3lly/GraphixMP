#pragma once
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class DirectionLight : public Light
{
	//Direction light only needs direction aside from other light properties
public:
	glm::vec3 direction;

	DirectionLight(float xPos, float yPos, float zPos)
	{
		lightPosition = glm::vec3(xPos, yPos, zPos);
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		ambientStr = 1.0f;
		specPhog = 3.0f;
		specStr = 0.5f;
	}

	void setDirectionLight(GLuint shaderProgram)
	{

		//Get Uniforms from Shader
		unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor2");
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

		unsigned int ambientStrLoc2 = glGetUniformLocation(shaderProgram, "ambientStr2");
		glUniform1f(ambientStrLoc2, ambientStr);

		unsigned int ambientColorLoc = glGetUniformLocation(shaderProgram, "ambientColor2");
		glUniform3fv(ambientColorLoc, 1, glm::value_ptr(ambientColor));

		unsigned int specStrLoc = glGetUniformLocation(shaderProgram, "specStr2");
		glUniform1f(specStrLoc, specStr);

		unsigned int specPhongLoc = glGetUniformLocation(shaderProgram, "specPhog2");
		glUniform1f(specPhongLoc, specPhog);

		unsigned int direcLoc = glGetUniformLocation(shaderProgram, "direction");
		glUniform3f(direcLoc, 0.0f, 0.0f, 0.0f);

	}
};
