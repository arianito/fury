#version 330 core

out vec4 glColor;


in vec3 fragmentPosition;
in vec3 fragmentNormal;

uniform vec3 lightPosition = vec3(3, 4, 5);

vec3 lightColor = vec3(1, 1, 1);
vec3 objectColor = vec3(1, 0.5, 1);

vec3 ambientColor = vec3(1, 1, 1);
float ambientIntensity = 0.1;

void main() {


	vec3 norm = normalize(fragmentNormal);
    vec3 lightDir = normalize(lightPosition - fragmentPosition);

    vec3 diffuse = max(dot(lightDir, norm), 0.0) * lightColor * 0.9;
    vec3 ambient = ambientColor * ambientIntensity;


    vec3 result = (ambient + diffuse) * objectColor;
    glColor = vec4(result, 1.0);


}