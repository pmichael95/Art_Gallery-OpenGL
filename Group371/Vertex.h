#ifndef VERTEX_H
#define VERTEX_H

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// A simple structure to hold our vertex
struct Vertex {
	glm::vec4 position;
	glm::vec4 color;
	glm::vec3 normal;

	// Work in 4 dimensions for easier matrix multiplication
	// Default since we provided other ctors
	Vertex() {
		this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->normal = glm::vec3(0, 0, 0);
	}; 

	Vertex(glm::vec4 position) {
		this->position = position;
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->normal = glm::vec3(0, 0, 0);
	};

	Vertex(glm::vec3 position) {
		this->position = glm::vec4(position, 1.0f);
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->normal = glm::vec3(0, 0, 0);
	};

	Vertex(glm::vec4 position, glm::vec4 color) {
		this->position = position;
		this->color = color;
		this->normal = glm::vec3(0, 0, 0);

	};

	Vertex(glm::vec3 position, glm::vec3 color) {
		this->position = glm::vec4(position, 1.0);
		this->color = glm::vec4(color, 1.0f);
		this->normal = glm::vec3(0, 0, 0);
	};

	bool operator==(const Vertex& other) const {
		return position == other.position && color == other.color && normal == other.normal;
	}
	bool operator!=(const Vertex& other) const {
		return !(*this == other);
	}
};

// Convert a 3 float  string to a vertex
//inline Vertex toVertex(std::string str) {
//	Vertex v;
//	std::vector<GLfloat> split;
//	std::istringstream iss(str);
//	for (std::string s; iss >> s;) {
//		split.push_back(stof(s));
//	}
//	v.position = glm::vec4(split.at(0), split.at(1), split.at(2), 0);
//	return v;
//};

#endif // !VERTEX_H