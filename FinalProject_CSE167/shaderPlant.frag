#version 330 core
// This is a sample fragment shader.

uniform vec3 lightPos;
uniform vec3 viewPos;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 outColor;
in vec3 aNorm;
in vec3 fragPos;
in vec2 texCoords;

uniform sampler2D tex_flag;
//uniform int flag;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
//    color = vec4(outColor, sampleExtraOutput);
    
    // texture for flag only
    vec3 flagColor = texture(tex_flag, texCoords).rgb;
    
    vec3 norm = normalize(aNorm);
    
    //////lighting//////
    vec3 ks = vec3(1.0f, 1.0f, 1.0f);
    vec3 kd = vec3(1.0f, 1.0f, 1.0f);
    vec3 ka = vec3(1.0f, 1.0f, 1.0f);
    
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
//    vec3 objColor = vec3(1.0f, 1.0f, 1.0f);
    
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
//    if (flag == 1) {
//        vec3 result = (ambient + diffuse + specular) * flagColor;
//    } else {
        vec3 result = (ambient + diffuse + specular) * outColor;
//    }
    color = vec4(result, 1.0f);
}
