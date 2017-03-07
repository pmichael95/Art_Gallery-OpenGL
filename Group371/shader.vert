#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

out vec4 vertexColor;
  
uniform mat4 transform;

void main()
{
    gl_Position = transform * position;
    //vertexColor = vec4(normal, 1.0);
	vertexColor = color;
  
} 