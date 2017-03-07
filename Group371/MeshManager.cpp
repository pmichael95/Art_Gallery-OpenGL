#include "MeshManager.h"


MeshManager::MeshManager() {
	verticeCount = 0;
	indiceCount = 0;
}
MeshManager::~MeshManager() {
}


void MeshManager::addMesh(Mesh* m) {
	// Add the mesh to our array
	meshes.push_back(m);
	// Update the reserve count for the vertices and indices
	verticeCount += m->vertices.size();
	indiceCount += m->indices.size();
}

void MeshManager::computeMergedMesh() {
	// Reserve space for our vertices
	finalMesh.vertices.reserve(verticeCount);
	finalMesh.indices.reserve(indiceCount);
	int offset = 0;
	for (Mesh* m : meshes) {
		// Copy vertices
		for (Vertex v : m->vertices) {
			finalMesh.vertices.push_back(v);
		}

		// Copy indices
		for (int i : m->indices) {
			finalMesh.indices.push_back(offset + i);
		}
		offset += m->vertices.size();
	}
}
