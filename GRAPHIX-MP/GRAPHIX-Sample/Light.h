#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Light
{
	//Light class members to be used for both point and direction light
public:
	glm::vec3 lightPosition;
	glm::vec3 lightColor;
	glm::vec3 ambientColor;
	float ambientStr;
	float specStr;
	float specPhog;
	//Initialize light values
	Light()
	{
		lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		ambientColor = lightColor;
		ambientStr = 0.2f;
		specPhog = 20.0f;
		specStr = 1.0f;

	}
	Light(float xPos, float yPos, float zPos)
	{
		lightPosition = glm::vec3(xPos, yPos, zPos);
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
		ambientStr = 0.2f;
		specPhog = 20.0f;
		specStr = 1.0f;
	}
};
