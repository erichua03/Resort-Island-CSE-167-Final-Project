#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model; //toWorld

uniform float terrain_width;

out vec3 terrainNormal;
out vec2 texCoords_2D;
//out vec3 texCoords_3D;
out vec3 fragPos;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    fragPos = vec3(model * vec4(position, 1.0f));
    
    terrainNormal = mat3(transpose(inverse(model))) * normal;
//    terrainNormal = normal;
    
    texCoords_2D = vec2(position.x, position.z);
    
//    texCoords_3D = position;
}
