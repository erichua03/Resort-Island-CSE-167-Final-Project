#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.


layout (location = 0) in vec3 position;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 toWorld;
uniform vec3 color;
uniform int flag;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out float sampleExtraOutput;
out vec3 outColor;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview *toWorld*vec4(position.x, position.y, position.z, 1.0);
    sampleExtraOutput = 1.0f;
    if (flag==1){
        if (position.x>15&&position.x<30){
            outColor = color;
        }
        else if (position.x>30){
            outColor = vec3(0.6f,0.8f,0.9f);
        }
        else{
            outColor = vec3(0.6f,0.1f,0.9f);
        }
    }
    else{
        outColor = color;
    }
}
