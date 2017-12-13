#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

//layout (location = 2) in vec3 tangent;
//layout (location = 3) in vec3 bitangent;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model; //toWorld

out vec3 terrainNormal;
out vec2 texCoords;
out vec3 fragPos;
//
//out mat3 TBN;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    fragPos = vec3(model * vec4(position, 1.0f));
    
//    terrainNormal = mat3(transpose(inverse(model))) * normal;
    terrainNormal = normal;
    
    texCoords = vec2(position.x, position.z);
    
//    // create a TBN matrix to get normal mapping working
//    vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
//    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0f)));
//    // or B = cross(N, T);
//    vec3 N = normalize(vec3(model * vec4(normal, 0.0f)));
//    TBN = mat3(T, B, N);
    
    // Two ways to use TBN matrix for normal mapping (second way preferred)
    
    // 1. Take a TBN matrix that transforms any vector from tangent to world space
    // Give it to fragment shader, and transform the sampled normal from tangent space to world space using TBN
    // The normal is then the same space as the other lighting variables
    
    // 2. Take the inverse of TBN matrix that transforms any vector from world space to tangent space
    // Use this matrix to transform not normal, but other relevant lighting variables to tangent space
    // The normal is then again in the same space as the other lighting variables
}
