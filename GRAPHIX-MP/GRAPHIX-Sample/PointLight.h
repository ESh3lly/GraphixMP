#pragma once
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class PointLight : public Light
{
	//Point light members
public:
	float constant = 1.0f;
	float linear = 0.5f;
	float quadratic = 0.1f;

	PointLight(float xPos, float yPos, float zPos)
	{
		lightColor = glm::vec3(1, 1, 1);
		lightPosition = glm::vec3(xPos, yPos, zPos);
		// Init to 0 for unlit
		ambientStr = 0.0f;
		specPhog = 10.0f;
		specStr = 0.0f;
	}

	void setPointLight(GLuint shaderProgram)
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
	}


};
