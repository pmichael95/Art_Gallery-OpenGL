#include "RandomShape.h"

// Outside faces are culled
RandomShape::RandomShape() 
{
	// The mesh manager
	MeshManager meshManager;

	/* initialize random seed: */
	int randnum;
	srand(time(NULL));
	randnum = rand() % 4;

	/* generate secret number between 0 and 1: */
	for (int i = 0; i < 1; i++)
	{
		Mesh m;
		Shapes shape = Shapes(3);
		switch (shape)
		{
		case CUBE:
			m = Cube();
			m.localRotateMesh(glm::radians(360.0f / randnum), glm::vec3(1, 1, 1));
			//m.localScaleMesh(glm::vec3(randnum + 1.0f, 1, randnum + 1.0f));
			break;
		case CYLINDER:
			m = Cylinder();
			m.localRotateMesh(glm::radians(180.0f / randnum), glm::vec3(1, 1, 1));
			//m.localScaleMesh(glm::vec3(randnum + 1.0f, 1, randnum + 1.0f));
			//m.translateMesh(glm::vec3(1, 0, 1));
			break;
		case CONE:
			m = Cone();
			//m.localRotateMesh(glm::radians(360.0f/randnum), glm::vec3(1, 1, 1));
			//m.localScaleMesh(glm::vec3(randnum + 1.0f, 1, randnum + 1.0f));
			m.translateMesh(glm::vec3(0, 1, 0));
			break;
		case SUPER:
			m = Superform();
			break;
		default:
			//m = Plane();
			break;
		}
		meshManager.addMesh(m);
	}
	meshManager.computeMergedMeshCopies();
	Mesh finalm = meshManager.finalMesh;
	
	vertices = finalm.getVertices();
	indices = finalm.getIndices();
}


RandomShape::~RandomShape() {
}
Mesh RandomShape::Superform()
{
	Mesh mesh;
	//Superformula Variables
	//m,n1,n2,n3,longitude,a,b
	//m,n1,n2,n3,latitude,a,b
	//double m = 10, n1 = 1, n2 = 1, n3 = 1, a = 1, b = 1;

	double m = (rand() % 50) + 1, n1 = (rand() % 10) + 1, n2 = (rand() % 10) + 1, n3 = (rand() % 10) + 1, a = 1, b = 1;
	std::cout << "M=" << m << "N1=" << n1 << "N2=" << n2 << "N3=" << n3 << std::endl;
	const int size = 25;
	int indiceCounter = 0;

	//Cartesian Planes Coordinates
	float x, y, z; 

	// Draw the superformula using Spherical(Polar) Coordinates
	const float PI = 3.1459;
	const float HALF_PI = 3.1459 / 2;

	//Loop Angle on Theta - Θ 
	for (int i = 0; i < size; i++)
	{
		// Map the latitude Theta - Θ 
		float latitude = map(i, 0, size, -HALF_PI, HALF_PI);

		//Loop Angle on Phi - Φ
		for (int j = 0; j < size; j++)
		{
			// Map the longitude Phi - Θ 
			float longitude = map(j, 0, size, -PI, PI);
			
			//Get the Two Superformulas.
			float r1 = superformula(m, n1, n2, n3, longitude, a, b);
			float r2 = superformula(m, n1, n2, n3, latitude, a, b);

			// Convert from Spherical (Polar) to Cartesian coordinates.
			x = r1 * cos(longitude) * r2 * cos(latitude);
			y = r1 * sin(longitude) * r2 * cos(latitude);
			z = r2 * sin(latitude);

			// Make a Vertex out of the coordinates and add them to the mesh.
			Vertex vertex = glm::vec3(x, y, z);
			mesh.addVertex(vertex);
		}
	}
	//Compute the indices for the Supershape.
	for (int i = 0; i < (size*size)-size-1; i++) 
	{
		mesh.addIndices(glm::vec3(i, i + 1, i + (size)));
		mesh.addIndices(glm::vec3(i + 1, i + (size) + 1 , i + (size)));
	}
	return mesh;
}
float RandomShape::superformula(double m, double n1, double n2, double n3, double angle, float a, float b)
{
	return (pow((pow(abs(cos(m*angle / 4.0f) / a), (n2)) + pow(abs(sin(m*angle / 4.0f) / b), (n3))), (-1.0f / n1)));
}
float RandomShape::map(float value, float istart, float istop, float ostart, float ostop)
{
	return (ostart + (ostop - ostart) * ((value - istart) / (istop - istart)));
}