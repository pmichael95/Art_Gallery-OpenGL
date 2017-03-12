#include "Display.h"

Display::Display(std::string title, int width, int height, bool enableDebugging) {
	debugModeEnabled = enableDebugging;
	initGL(title, width, height);
	initGLBuffers();
	initSharedData();
	initCamera();
	if (debugModeEnabled){
		initDebugMeshes();
	}


	Cube c;
	c.translateMesh(glm::vec3(0.5f, 0.5f, 0.5f));
	c.localRotateMesh(glm::radians(45.0), glm::vec3(1, 1, 1));

	Plane p;
	p.translateMesh(glm::vec3(5, 0, 5));
	p.localScaleMesh(glm::vec3(5, 1, 5));
	p.localRotateMesh(glm::radians(45.0), glm::vec3(0, 1, 0));

	Circle cc;

	Cone cy(20);
	//cy.translateMesh(glm::vec3(2, 2, 2));
	//cy.rotateMesh(glm::radians(-45.0), glm::vec3(0, 1, 0));
	//cy.localScaleMesh(glm::vec3(2, 10, 2));

	//meshManager.addMesh(&c);
	//meshManager.addMesh(&p);
	//meshManager.addMesh(&cc);
	meshManager.addMesh(&cy);


	// TODO: procedurally generate all the mesh data here
	// generateEnivorment();
	meshManager.computeMergedMesh();
}


Display::~Display() {
	//delete buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	if (debugModeEnabled) {
		glDeleteBuffers(1, &DEBUG_VBO);
		glDeleteBuffers(1, &DEBUG_EBO);
	}
	glfwTerminate();
}

void Display::update() {

	// poll events from kb/mouse or other inputs
	glfwPollEvents();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// clear color buffer and depth
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the view matrix
	sharedData.transform->view = sharedData.camera->getViewMatrix();

	// Compute the cmt transform and pass to our shader
	glUniformMatrix4fv(transformUniform, 1, GL_FALSE, value_ptr(sharedData.transform->getTransform()));

	shader->Use();

	glBindVertexArray(VAO);
	
	if (debugModeEnabled) {
		debugUpdate();
	}
	// Draw the rest of our scene
	glBufferData(GL_ARRAY_BUFFER, meshManager.finalMesh.vertexBufferSize(), meshManager.finalMesh.getVertices().data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshManager.finalMesh.indexBufferSize(), meshManager.finalMesh.getIndices().data(), GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, meshManager.finalMesh.getIndices().size(), GL_UNSIGNED_INT, 0);
	
	
	// swap the buffer (double buffer)
	glfwSwapBuffers(window);
}

bool Display::isClosed() {
	return glfwWindowShouldClose(window);
}

void Display::clear(float r, float g, float b, float a) {
	//clear screen
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::setShader(Shader* shader) {
	this->shader = shader;
	// Get the uniform locations only once when we set up the shader
	transformUniform = glGetUniformLocation(shader->Program, "transform");
}

void Display::setHandler(GLFWCallbackHandler* handler) {
	this->handler = handler;
	this->handler->attachToWindow(window);
	if (this->handler->callback != NULL) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		// Set the initial width and height in the callback so we can respond to window resizes properly
		this->handler->callback->setLastWindowSize(width, height);
		// Set the shared data between the display and the callback
		this->handler->callback->setSharedData(&sharedData);
	}
}

void Display::initGL(std::string windowName, int width, int height) {
	// init glfw 
	glfwInit();

	// specify version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// use core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// set resizeable
	glfwWindowHint(GLFW_RESIZABLE, true);

	// Create a glfw window and set its height and width
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

	// If we don't get a window, then it failed and we should terimnate and free the resources
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	// init glew and set experimental glew to true to allow for modern graphics techniques
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}

	// set our viewport to the size of the window
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Enable face culling
	// TODO: should we enable this?
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}

void Display::initCamera() {
	camera = Camera();
	sharedData.transform->model = glm::mat4();
	sharedData.transform->view = sharedData.camera->getViewMatrix();
	sharedData.transform->projection = sharedData.camera->getProjectionMatrix();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Display::initGLBuffers() {
	// set up vertex buffers
	// Generate the vertex array
	glGenVertexArrays(1, &VAO);
	// Bind vertex array to buffer
	glBindVertexArray(VAO);
	// Generate element buffer object
	glGenBuffers(1, &EBO);
	// Generate vertex buffer object
	glGenBuffers(1, &VBO);
	// bind vertex buffer object to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// bind element buffer object to buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	if (debugModeEnabled) {
		// Generate element buffer object
		glGenBuffers(1, &DEBUG_EBO);
		// Generate vertex buffer object
		glGenBuffers(1, &DEBUG_VBO);
		// bind vertex buffer object to buffer
		glBindBuffer(GL_ARRAY_BUFFER, DEBUG_VBO);
		// bind element buffer object to buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DEBUG_EBO);
	}

	// Position attribute
	GLuint byteOffset = 0;
	// Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)byteOffset);
	glEnableVertexAttribArray(0);

	byteOffset += sizeof(glm::vec4);
	// Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(byteOffset));
	glEnableVertexAttribArray(1);

	byteOffset += sizeof(glm::vec4);
	// Normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(byteOffset));
	glEnableVertexAttribArray(2);

}

void Display::initSharedData() {
	sharedData.transform = &transform;
	sharedData.camera = &camera;
	sharedData.meshManager = &meshManager;
}

void Display::initDebugMeshes() {
	GridMesh grid;
	debugMeshManager.grid = grid;
	XYZAxis xyz;
	debugMeshManager.axes = xyz;
}

void Display::debugUpdate() {
	// Draw grid
	glBindBuffer(GL_ARRAY_BUFFER, DEBUG_VBO);
	glBufferData(GL_ARRAY_BUFFER, debugMeshManager.grid.vertexBufferSize(), debugMeshManager.grid.getVertices().data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DEBUG_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, debugMeshManager.grid.indexBufferSize(), debugMeshManager.grid.getIndices().data(), GL_STATIC_DRAW);
	glDrawElements(GL_LINES, debugMeshManager.grid.getIndices().size(), GL_UNSIGNED_INT, 0);


	glLineWidth(5);
	glBindBuffer(GL_ARRAY_BUFFER, DEBUG_VBO);
	glBufferData(GL_ARRAY_BUFFER, debugMeshManager.axes.vertexBufferSize(), debugMeshManager.axes.getVertices().data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DEBUG_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, debugMeshManager.axes.indexBufferSize(), debugMeshManager.axes.getIndices().data(), GL_STATIC_DRAW);
	glDrawElements(GL_LINES, debugMeshManager.axes.getIndices().size(), GL_UNSIGNED_INT, 0);

	glLineWidth(1);
	
}
