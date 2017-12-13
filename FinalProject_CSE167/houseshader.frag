#version 330 core
// This is a sample fragment shader.


// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

struct Light{
// 0 direction light 1 point light 2 cone
	int light_mode;
	vec3 light_color;
	vec3 light_pos;
	vec3 light_dir;

	float linear_att;
	float quad_att;

	float cutoff_angle;
	float exponent;
	mat4 transform;
};

struct Material {
	int obj_mode;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;
uniform Material material;
uniform vec3 cam_pos;

in vec3 Normal;
in vec3 FragPos;
in vec3 origin;

out vec4 color;


void main()
{
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    // normal
    vec3 norm = normalize(Normal);
    vec3 camPos = vec3(0.0f, 0.0f, 20.0f);
    vec3 viewDir = normalize(camPos - FragPos);
    if (material.obj_mode == 0)    {
        color = vec4(norm.x, norm.y, norm.z, 1.0f);
    } else if (light.light_mode == 0) {

        mat3 lightTransform = transpose(inverse(mat3(light.transform)));

        vec3 lightDir = normalize(lightTransform*-light.light_dir);

        float diff = max(dot(norm,lightDir),0.0f);

        vec3 ambient = light.light_color * material.ambient;

        vec3 reflectDir = reflect(-lightDir,norm);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 diffuse = light.light_color * material.diffuse * diff;

        vec3 specular = light.light_color * material.specular * spec;

        color = vec4(specular + diffuse + ambient, 1.0f);
    }

    else if (light.light_mode == 1) {

        vec3 lightPos = vec3(light.transform * vec4(light.light_pos,1));

        vec3 lightDir = normalize(lightPos - FragPos);

        float diff = max(dot(norm,lightDir),0.0f);



        vec3 reflectDir = reflect(-lightDir,norm);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 diffuse = light.light_color * material.diffuse * diff;

        vec3 specular = light.light_color * material.specular * spec;

        vec3 attenuation = light.light_color / (light.linear_att * length(lightPos - FragPos));
        vec3 ambient = light.light_color * material.ambient;
        ambient = ambient * attenuation;
        diffuse = diffuse * attenuation;
        specular = specular * attenuation;

        color = vec4(specular + diffuse + ambient, 1.0f);
    }

    else if (light.light_mode == 2) {

        vec3 lightPos = vec3(light.transform* vec4(light.light_pos,1));

        vec3 lightDir = normalize(lightPos - FragPos);


        float angle = dot(-normalize(light.light_dir),lightDir);
        vec3 src;
        if (angle >= light.cutoff_angle ){
            src = light.light_color * pow(angle,light.exponent);
        } else {
            src = vec3(0.0f,0.0f,0.0f);
        }
        vec3 attenuation = src / (light.quad_att * length(lightPos - FragPos) * length(lightPos - FragPos));

        float diff = max(dot(norm,lightDir),0.0f);

        vec3 ambient = light.light_color * material.ambient;

        vec3 reflectDir = reflect(-lightDir,norm);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 diffuse = light.light_color * material.diffuse * diff;

        vec3 specular = light.light_color * material.specular * spec;


        ambient = ambient * attenuation;
        diffuse = diffuse * attenuation;
        specular = specular * attenuation;

        color = vec4(specular + diffuse + ambient, 1.0f);
    }
    
}
