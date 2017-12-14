#version 330 core

in vec3 terrainNormal;
in vec2 texCoords;
in vec3 fragPos;
//
in mat3 TBN;
in vec3 glPos;

out vec4 fragColor;

// values that stay constant for the whole mesh
uniform sampler2D texture_0; // rock
uniform sampler2D texture_1; // grass
uniform sampler2D texture_2; // snow
//
uniform sampler2D normalMap; // normal mapping
uniform float renderHeight; //

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 texColor_0 = texture(texture_0, texCoords).rgb;
    vec3 texColor_1 = texture(texture_1, texCoords).rgb;
    vec3 texColor_2 = texture(texture_2, texCoords).rgb;
    vec3 texColor_nm = texture(normalMap, texCoords).rgb;
    vec3 texColor;
    
    // mixed texture mapping on varying heights
    float height = fragPos.y / terrainNormal.y;
    
    const float range1 = -60.0f;
    const float range2 = -5.0f;
    const float range3 = 3.0f;
    const float range4 = 30.0f;
    
    if (height < range1) {
        texColor = texColor_1;
    }
    else if (height >= range1 && height < range2) {
        texColor = mix(texColor_1, texColor_0, (height-range1)/(range2-range1));
    }
    else if (height >= range2 && height < range3) {
        texColor = texColor_0;
    }
    else if (height >= range3 && height < range4) {
        texColor = mix(texColor_0, texColor_2, (height-range3)/(range4-range3));
    }
    else {
        texColor = texColor_2;
    }
    
    //////normal mapping//////
//    vec3 norm = texture(normalMap, texCoords).rgb;
//    norm = normalize(norm * 2.0f - 1.0f);
//    norm = normalize(TBN * norm);
//    vec3 norm = normalize(texture(normalMap, texCoords).xyz) * 2.0f - 1.0f;
    
    vec3 norm = normalize(terrainNormal); //

    //////lighting//////
    vec3 ks = vec3(1.0f, 1.0f, 1.0f);
    vec3 kd = vec3(1.0f, 1.0f, 1.0f);
    vec3 ka = vec3(1.0f, 1.0f, 1.0f);

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objColor = vec3(1.0f, 1.0f, 1.0f);

//    vec3 lightVec = lightPos * TBN;
    vec3 lightDir = normalize(lightPos - fragPos);
//    vec3 lightDir = normalize(lightVec - fragPos);

    // Diffuse color
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // Ambient color
    vec3 ambient = ka * lightColor; // ka * ca

    // Specular color
    vec3 viewDir = normalize(viewPos - fragPos); // e;
//    vec3 reflectDir = reflect((1.0f) * lightDir, normalize(terrainNormal)); // R = -L;
    vec3 reflectDir = reflect((-1.0f) * lightDir, norm); // R = -L; or -lightDir?
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 80.0f); // (e * R)^p
    vec3 specular = ks * spec * lightColor; // ks * cl * (e * R) ^ p

    // Final color
//    vec3 result = (ambient + diffuse + specular) * texColor_nm;
    vec3 result = (ambient + diffuse + specular) * texColor;
    fragColor = vec4(result, 1.0f);
    
}








