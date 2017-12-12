#version 330 core

in vec3 terrainNormal;
in vec2 texCoords_2D;
//in vec3 texCoords_3D;
in vec3 fragPos;

out vec4 fragColor;

// values that stay constant for the whole mesh
uniform sampler2D texture_0; // rock
uniform sampler2D texture_1; // grass

uniform sampler2D normalMap; // normal mapping

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    fragColor = texture(texture_0, texCoords_2D);
    
    vec3 texColor = texture(texture_0, texCoords_2D).rgb;
    
//    vec3 ks = vec3(0.628281f, 0.555802f, 0.366065f); // have significant diffuse and specular reflection components
//    vec3 kd = vec3(0.75164f, 0.60648f, 0.22648f);
//    vec3 ka = vec3(0.24725f, 0.1995f, 0.0745f); // gold
    
    vec3 ks = vec3(1.0f, 1.0f, 1.0f);
    vec3 kd = vec3(1.0f, 1.0f, 1.0f);
    vec3 ka = vec3(1.0f, 1.0f, 1.0f);
    
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightDir = normalize(lightPos - fragPos);
    
    vec3 norm = normalize(terrainNormal);
    
    // Normal mapping
//    vec3 rgbNormal = terrainNormal * 0.5f + 0.5f; // transforms from [-1,1] to [0,1]
//    norm = texture(normalMap, texCoords_2D).rgb; // obtain normal from normal map in range [0,1]
//    norm = normalize(terrainNormal * 2.0f - 1.0f); // transform normal vector to range [-1,1]
    
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
//    vec3 result = (ambient + diffuse + specular) * objColor;
    vec3 result = (ambient + diffuse + specular) * texColor;
//    fragColor = vec4(result, 1.0f);
    
}








