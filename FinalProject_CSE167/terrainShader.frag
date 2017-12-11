#version 330 core

in vec3 terrainNormal;
//in vec3 TexCoords;
in vec2 texCoords;

out vec4 fragColor;

// values that stay constant for the whole mesh
uniform sampler2D texSampler;

void main()
{
//    vec3 norm = (normalize(terrainNormal)+1.0f)/2.0f; // only for normal coloring
//    vec3 norm = (terrainNormal+1.0f)/2.0f; // only for normal coloring
//    fragColor = vec4(norm, 1.0f); // normal coloring
    
//    vec3 color = vec3(1.0f, 1.0f, 1.0f);
//    fragColor = vec4(color, 1.0f);
    
    fragColor = texture(texSampler, texCoords);
}








