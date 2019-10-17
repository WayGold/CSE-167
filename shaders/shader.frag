#version 330 core
// This is a sample fragment shader.

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in float sampleExtraOutput;
in vec3 normalOutput;
//in vec3 posOutput;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float light_linear;

uniform int flag;

uniform Material material;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 result;
    
    if(flag == 1){
        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
          
        // diffuse
        vec3 norm = normalize(normalOutput);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * material.diffuse;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = specularStrength * spec * lightColor * material.specular;
        
        // attenuation
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (light_linear * distance);

        ambient  *= attenuation;
        diffuse   *= attenuation;
        specular *= attenuation;
            
        result = (ambient + diffuse + specular) * objectColor;
    }
    else{
        result = vec3((normalOutput.x + 1) / 2, (normalOutput.y + 1) / 2, (normalOutput.z + 1) / 2);
        result = normalize(result);
    }

    fragColor = vec4(result, 1.0);
}
