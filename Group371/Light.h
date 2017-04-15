#ifndef LIGHT_H
#define LIGHT_H
#include <glm/detail/type_vec3.hpp>

struct Light {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define LIGHT_DISTANCE_13 Light {glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.35f, 0.44f};
#define LIGHT_DISTANCE_20 Light {glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.22f, 0.20f};
#define LIGHT_DISTANCE_32 Light {glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.14f, 0.07f};
#define LIGHT_DISTANCE_41 Light {glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.115f, 0.051f};
#define LIGHT_DISTANCE_50 Light {glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f};
#define LIGHT_DISTANCE_65 Light {glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.07f, 0.017f};

#endif

