#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightModel;
uniform float sourceLight;

out vec3 Normal;
out vec3 FragPos;
out vec3 origin;
out vec4 sourceColor;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    
    FragPos = vec3(model * vec4(position, 1.0f));
	Normal = transpose(inverse(mat3(model))) * normal;
	origin = vec3(model*vec4(0.0f,0.0f,0.0f,1));
	sourceColor = vec4(sourceLight,sourceLight,sourceLight,sourceLight);
}
