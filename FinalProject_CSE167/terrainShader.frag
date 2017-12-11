#version 330 core

in vec3 terrainNormal;
//in vec2 texCoords;
in vec3 fragPos;

out vec4 fragColor;

// values that stay constant for the whole mesh
uniform sampler2D texture_0; // rock
uniform sampler2D texture_1; // grass
//uniform sampler2D texture_2; // snow

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
//    vec3 norm = (normalize(terrainNormal)+1.0f)/2.0f; // only for normal coloring
//    vec3 norm = (terrainNormal+1.0f)/2.0f; // only for normal coloring
//    fragColor = vec4(norm, 1.0f); // normal coloring
    
//    fragColor = texture(texture_1, texCoords);
    
//    vec3 color = vec3(0.6f, 0.5f, 1.0f);
//    fragColor = vec4(color, 1.0f);
    
    vec3 ks = vec3(0.628281f, 0.555802f, 0.366065f); // have significant diffuse and specular reflection components
    vec3 kd = vec3(0.75164f, 0.60648f, 0.22648f);
    vec3 ka = vec3(0.24725f, 0.1995f, 0.0745f); // gold
    
//    vec3 ks = vec3(0.0f, 0.0f, 0.0f);
//    vec3 kd = vec3(0.0f, 0.0f, 0.0f);
//    vec3 ka = vec3(0.0f, 0.0f, 0.0f);
    
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightDir = normalize(lightPos - fragPos);
    
    vec3 norm = normalize(terrainNormal);
    
    // Diffuse color
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    
    // Ambient color
    vec3 ambient = ka * lightColor; // ka * ca
    
    // Specular color
    vec3 viewDir = normalize(viewPos - fragPos); // e;
    vec3 reflectDir = reflect((1.0f) * lightDir, normalize(terrainNormal)); // R = -L;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 10); // (e * R)^p
    vec3 specular = ks * spec * lightColor; // ks * cl * (e * R) ^ p

    // Final color
    vec3 result = (ambient + diffuse + specular) * objColor;
    fragColor = vec4(result, 1.0f);

}








