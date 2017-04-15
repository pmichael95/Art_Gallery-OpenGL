#include "RandomShape.h"

// Outside faces are culled
RandomShape::RandomShape() 
{
	// The mesh manager
	MeshManager meshManager;

	//Get a Mesh (Supershaped)
	Mesh m;
	m = Superform();
	
	// Add the mesh to the manager and compute it
	meshManager.addMesh(m);
	meshManager.computeMergedMeshCopies();
	
	// Get the final Mesh.
	Mesh finalm = meshManager.finalMesh;
	
	// Set the vertices and indices.
	vertices = finalm.getVertices();
	indices = finalm.getIndices();
}
Mesh RandomShape::Superform()
{
	//Object that will hold the final shape.
	Mesh mesh;

	// Superformula variables
	double m;
	double n1;
	double n2;
	double n3;

	// Get the type of Supershape
	enum SuperShapeType { PINCHED, ASYMMETRIC, STAR, SYMMETRICAL };
	int shapeType = (rand() % 4);

	//Get a random type of Supershape
	switch (shapeType)
	{
	case SYMMETRICAL:
		//SYMMETRICAL Shape
		std::cout << "SYMMETRICAL" << std::endl;
		m = (rand() % 10) + (rand() % 10 + 1);
		n1, n2, n3 = (rand() % 7) + (rand() % 10 + 1);
		break;
	case PINCHED:
		//PINCHED Shape
		std::cout << "PINCHED" << std::endl;
		m = (rand() % 7) + (rand() % 10 + 1);
		n1 = n2 = n3 = (rand() % 100) / 100.0;
		break;
	case ASYMMETRIC:
		//ASYMMETRIC Shape
		std::cout << "ASYMMETRIC" << std::endl;
		m = (rand() % 10) + (rand() % 10 + 1);
		n1 = (rand() % 100) + (rand() % 10 + 1);
		n2 = (rand() % 100) + (rand() % 10 + 1);
		n3 = (rand() % 100) + (rand() % 10 + 1);
		break;
	case STAR:
		//STAR Shape
		std::cout << "STAR" << std::endl;
		m = 5;
		n1 = (rand() % 100) / 100.0;
		n2 = n3 = (rand() % 100) / 100.0;
		break;
	default:
		break;
	}
	//std::cout << "M=" << m << "N1=" << n1 << "N2=" << n2 << "N3=" << n3 << std::endl; Comment this out to check xyz of the shape
	double a = 1, b = 1;
	const int size = 20;


	//Cartesian Planes Coordinates
	float x, y, z;

	//Object that will hold the xyz
	Vertex vertex;

	// Draw the superformula using Spherical(Polar) Coordinates using Pi values
	const float PI = 3.1459;
	const float HALF_PI = 3.1459 / 2;

	//Loop Angle on Theta - Θ 
	for (int i = 0; i <= size; i++)
	{
		// Map the latitude Theta - Θ 
		float latitude = map(i, 0, size, -HALF_PI, HALF_PI);

		//Loop Angle on Phi - Φ
		for (int j = 0; j < size; j++)
		{
			// Map the longitude Phi - Θ 
			float longitude = map(j, 0, size, -PI, PI);

			//Get the Two Superformulas to go from 2D to 3D.
			float r1 = superformula(m, n1, n2, n3, longitude, a, b);
			float r2 = superformula(m, n1, n2, n3, latitude, a, b);

			// Convert from Spherical (Polar) to Cartesian 3D coordinates.
			x = r1 * cos(longitude) * r2 * cos(latitude);
			y = r1 * sin(longitude) * r2 * cos(latitude);
			z = r2 * sin(latitude);

			// Make a Vertex out of the coordinates and add them to the mesh.
			vertex = glm::vec3(x, y, z);

			// Normalize the Vector.
			vertex.position = glm::normalize(vertex.position);

			// Make shapes not exceed length of 1
			vertex.position = glm::vec4(vertex.position.x / 2, vertex.position.y / 2, vertex.position.z / 2, vertex.position.w);
			
			if (vertex.position.x > 0.5f)
				vertex.position.x = 0.5f;
			if (vertex.position.x < -0.5f)
				vertex.position.x = -0.5f;
			if (vertex.position.y > 0.5f)
				vertex.position.y = 0.5f;
			if (vertex.position.y < -0.5f)
				vertex.position.y = -0.5f;
			if (vertex.position.z > 0.5f)
				vertex.position.z = 0.5f;
			if (vertex.position.z < -0.5f)
				vertex.position.z = -0.5f;
				

			//std::cout << "X: " << vertex.position.x << " Y: " << vertex.position.y << " Z: " << vertex.position.z << std::endl;

			// Add the vertex to the mesh.
			mesh.addVertex(vertex);
		}
	}
	//Compute the indices for the Supershape.
	for (int i = 0; i < (size*size) - 1; i++)
	{
		mesh.addIndices(glm::vec3(i, i + 1, i + (size)));
		mesh.addIndices(glm::vec3(i + 1, i + (size)+1, i + (size)));
	}
	return mesh;
}
// The Superformula (Johan Gielis's) https://en.wikipedia.org/wiki/Superformula
float RandomShape::superformula(double m, double n1, double n2, double n3, double angle, float a, float b)
{
	return (pow((pow(abs(cos(m*angle / 4.0f) / a), (n2)) + pow(abs(sin(m*angle / 4.0f) / b), (n3))), (-1.0f / n1)));
}
// Mapping function for floats.
float RandomShape::map(float value, float istart, float istop, float ostart, float ostop)
{
	return (ostart + (ostop - ostart) * ((value - istart) / (istop - istart)));
}