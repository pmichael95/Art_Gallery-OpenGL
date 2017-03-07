#include "Display.h"
#include "GLFWCallbackHandler.h"

int main() {
	Display display("Art Gallery", 800, 800, true);

	// Set the shader for this diplay
	Shader shader("shader.vert", "shader.frag");
	display.setShader(&shader);
	
	// Set up input handling
	GLFWCallbackHandler* handler = GLFWCallbackHandler::getInstance();
	GLFWCallback callback;
	handler->callback = &callback;
	display.setHandler(handler);


	// Main game loop
	while (!display.isClosed()) {
		//draw
		display.update();
		display.clear(0.0f, 0.0f, 1.0f, 1.0f);

	}

	return 0;
}
