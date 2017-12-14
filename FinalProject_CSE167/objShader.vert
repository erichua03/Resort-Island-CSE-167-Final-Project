#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model; //toWorld

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.

out vec3 fragPos; // fragTexCoord
out vec3 fragNormal;
out vec3 objNormal;

out vec2 texCoords;

out mat3 TBN;

void main()
{
    
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    fragNormal = aNormal;
    fragNormal = mat3(transpose(inverse(model))) * aNormal;
    objNormal = aNormal;
    
    fragPos = vec3(model * vec4(position, 1.0)); // world space
    
    texCoords = vec2(position.x, position.z);//
    
    // create a TBN matrix to get normal mapping working
    vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0f)));
    // or B = cross(N, T);
//        vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
}
