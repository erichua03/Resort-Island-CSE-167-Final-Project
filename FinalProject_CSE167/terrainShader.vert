#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model; //toWorld

out vec3 terrainNormal;
//out vec2 texCoords;
out vec3 fragPos;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    fragPos = vec3(model * vec4(position, 1.0f));
    
    terrainNormal = mat3(transpose(inverse(model))) * normal;

//    terrainNormal = normal;
//    texCoords = vec2(position.x, position.z);
}
