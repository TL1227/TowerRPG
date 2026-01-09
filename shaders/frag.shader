#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{    
    //FragColor = texture(texture_diffuse1, TexCoords);

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec4 tex = texture(texture_diffuse1, TexCoords);
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0) * tex;
}
