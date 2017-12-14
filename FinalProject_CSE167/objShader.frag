#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 fragNormal;
in vec3 objNormal;
in vec3 fragPos; // fragVert

in vec2 texCoords;
in mat3 TBN; //

uniform sampler2D normalMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 lightVec = normalize(lightPos * TBN);
    
    vec3 color = vec3(0.2f, 0.4f, 0.3f);
    
//    vec3 texColor_nm = texture(normalMap, texCoords).rgb;
//    fragColor = vec4(texColor_nm, 1.0f);
    
    //////normal mapping//////
//    vec3 norm = texture(normalMap, texCoords).rgb;
//    norm = normalize(norm * 2.0f - 1.0f);
//    norm = normalize(TBN * norm);
    
    vec3 norm = normalize(texture(normalMap, texCoords).xyz) * 2.0f - 1.0f;
//    norm = TBN * norm;
    
//    vec3 norm = (normalize(objNormal)+1.0f)/2.0f; // only for normal coloring
//    fragColor = vec4(norm, 1.0f); // normal coloring
    
    //////lighting//////
    vec3 ks = vec3(0.628281f, 0.555802f, 0.366065f); // have significant diffuse and specular reflection components
    vec3 kd = vec3(0.75164f, 0.60648f, 0.22648f);
    vec3 ka = vec3(0.24725f, 0.1995f, 0.0745f); // gold
    
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objColor = vec3(1.0f, 1.0f, 1.0f);
    
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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 10.0f); // (e * R)^p
    vec3 specular = ks * spec * lightColor; // ks * cl * (e * R) ^ p
    
    // Final color
    vec3 result = (ambient + diffuse + specular) * objColor;
    fragColor = vec4(result, 1.0f);
}








