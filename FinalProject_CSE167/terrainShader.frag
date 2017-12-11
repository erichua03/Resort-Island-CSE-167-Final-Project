#version 330 core

in vec3 terrainNormal;
in vec2 texCoords;

out vec4 fragColor;

// values that stay constant for the whole mesh
uniform sampler2D texture_0; // rock
uniform sampler2D texture_1; // grass
//uniform sampler2D texture_2; // snow

void main()
{
//    vec3 norm = (normalize(terrainNormal)+1.0f)/2.0f; // only for normal coloring
//    vec3 norm = (terrainNormal+1.0f)/2.0f; // only for normal coloring
//    fragColor = vec4(norm, 1.0f); // normal coloring
    
//    vec3 color = vec3(1.0f, 1.0f, 1.0f);
//    fragColor = vec4(color, 1.0f);
    
    fragColor = texture(texture_1, texCoords);
    
//    vec4 color_0 = texture(texture_0, texCoords);
//    vec4 color_1 = texture(texture_1, texCoords);

//    fragColor = (color_0 * color_1) / 2;
}








