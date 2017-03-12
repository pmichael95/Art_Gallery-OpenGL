#include "Mesh.h"


Mesh::Mesh() {
	updateVerticesFromTransform();
}

Mesh::~Mesh() {
}

Mesh Mesh::copy() {
	Mesh copiedMesh;
	copiedMesh.vertices = vertices;
	copiedMesh.indices = indices;
	copiedMesh.indiceGroups = indiceGroups;
	copiedMesh.transform = transform;
	return copiedMesh;
}

void Mesh::addVertex(Vertex v) {
	vertices.push_back(v);	
	maxXYZ.x = glm::max(maxXYZ.x, v.position.x);
	maxXYZ.y = glm::max(maxXYZ.y, v.position.y);
	maxXYZ.z = glm::max(maxXYZ.z, v.position.z);

	minXYZ.x = glm::min(minXYZ.x, v.position.x);
	minXYZ.y = glm::min(minXYZ.y, v.position.y);
	minXYZ.z = glm::min(minXYZ.z, v.position.z);
	recalculateCenter();
}

void Mesh::addIndices(glm::ivec3 tri) {
	indices.push_back(tri.x);
	indices.push_back(tri.y);
	indices.push_back(tri.z);
	indiceGroups.push_back(tri);

	Vertex& v1 = vertices.at(tri.x);
	Vertex& v2 = vertices.at(tri.y);
	Vertex& v3 = vertices.at(tri.z);

	glm::vec3 d1 = v1.position - v2.position;
	glm::vec3 d2 = v1.position - v3.position;
	glm::vec3 normal = glm::normalize(glm::cross(d1, d2));
	v1.normal = normal;
	v2.normal = normal;
	v3.normal = normal;

}


int Mesh::vertexBufferSize() {
	return vertices.size() * sizeof(Vertex);
}

int Mesh::indexBufferSize() {
	return indices.size() * sizeof(int);
}



void Mesh::scaleMesh(glm::vec3 scale) {
	transform = glm::scale(transform, scale);
	updateVerticesFromTransform();
}

void Mesh::localScaleMesh(glm::vec3 scale) {
	glm::mat4 scaleMatrix;
	// Rotates around center of bounding box
	glm::vec4 vecToOrigin = center - glm::vec4(0, 0, 0, 0);
	scaleMatrix = glm::translate(scaleMatrix, glm::vec3(vecToOrigin));
	scaleMatrix = glm::scale(scaleMatrix, scale);
	scaleMatrix = glm::translate(scaleMatrix, -glm::vec3(vecToOrigin));
	transform = scaleMatrix * transform;
	updateVerticesFromTransform();
	
}

void Mesh::rotateMesh(float radians, glm::vec3 rotate) {
	transform = glm::rotate(transform, radians, rotate);
	transformNormals(transform);
	updateVerticesFromTransform();
}

void Mesh::localRotateMesh(float radians, glm::vec3 rotate) {
	glm::mat4 rotationMatrix;
	// Rotates around center of bounding box
	glm::vec4 vecToOrigin = center - glm::vec4(0, 0, 0, 0);
	rotationMatrix = glm::translate(rotationMatrix, glm::vec3(vecToOrigin));
	rotationMatrix = glm::rotate(rotationMatrix, radians, rotate);
	rotationMatrix = glm::translate(rotationMatrix, -glm::vec3(vecToOrigin));
	transform = rotationMatrix * transform;
	transformNormals(transform);
	updateVerticesFromTransform();
}

void Mesh::translateMesh(glm::vec3 translate) {
	transform = glm::translate(transform, translate);
	updateVerticesFromTransform();
}

void Mesh::updateVerticesFromTransform() {
	maxXYZ = glm::vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	minXYZ = glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

	for (Vertex& v : vertices) {
		v.position = transform * v.position;
		maxXYZ.x = glm::max(maxXYZ.x, v.position.x);
		maxXYZ.y = glm::max(maxXYZ.y, v.position.y);
		maxXYZ.z = glm::max(maxXYZ.z, v.position.z);

		minXYZ.x = glm::min(minXYZ.x, v.position.x);
		minXYZ.y = glm::min(minXYZ.y, v.position.y);
		minXYZ.z = glm::min(minXYZ.z, v.position.z);
	}
	recalculateCenter();
	transform = glm::mat4();
}

void Mesh::recalculateCenter() {
	// Gets center of bounding box
	center = glm::vec4((maxXYZ.x + minXYZ.x) / 2.0,
		(maxXYZ.y + minXYZ.y) / 2.0,
		(maxXYZ.z + minXYZ.z) / 2.0
		, 0
	);
}

void Mesh::transformNormals(glm::mat4& transform) {
	for (int i = 0; i < indiceGroups.size(); i++) {
		glm::ivec3 tri = indiceGroups.at(i);
		Vertex& v1 = vertices.at(tri.x);
		Vertex& v2 = vertices.at(tri.y);
		Vertex& v3 = vertices.at(tri.z);

		glm::vec3 d1 = v1.position - v2.position;
		glm::vec3 d2 = v1.position - v3.position;
		glm::vec3 normal = glm::vec3(transform * glm::vec4(glm::cross(d1, d2), 1.0));
		v1.normal = normal;
		v2.normal = normal;
		v3.normal = normal;
	}
}


GridMesh::GridMesh() {
	// Size should be a power of 2
	int size = 512;
	// TODO: figure out if size should be a power of 2 for faster generation
	float divSize = 1;

	// Generate vertices
	for (int i = -(size / 2); i <= (size / 2); i++) {
		for (int j = -(size / 2); j <= (size / 2); j++) {
			
			Vertex v(glm::vec3(i * divSize, 0, j * divSize));
			v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			vertices.push_back(v);
		}
	}

	for (int i = 0; i < vertices.size(); i++) {
		
			if (i == 0 || i % (size + 1) != size) {
				indices.push_back(i);
				indices.push_back(i + 1);
			}

			if (i < (size * size) + size) {
				indices.push_back(i);
				indices.push_back(i + size + 1);
			}
	}



}

GridMesh::~GridMesh() {}

XYZAxis::XYZAxis() {

	// x is red
	Vertex xOrigin(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
	Vertex xAxis(glm::vec3(10, 0, 0), glm::vec3(1, 0, 0));

	// y is green
	Vertex yOrigin(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	Vertex yAxis(glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));

	// z is blue
	Vertex zOrigin(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	Vertex zAxis(glm::vec3(0, 0, 10), glm::vec3(0, 0, 1));

	int top = 0;
	vertices.push_back(xOrigin);
	vertices.push_back(xAxis);
	indices.push_back(top++);
	indices.push_back(top++);

	vertices.push_back(yOrigin);
	vertices.push_back(yAxis);
	indices.push_back(top++);
	indices.push_back(top++);

	vertices.push_back(zOrigin);
	vertices.push_back(zAxis);
	indices.push_back(top++);
	indices.push_back(top++);
}


XYZAxis::~XYZAxis() {
}