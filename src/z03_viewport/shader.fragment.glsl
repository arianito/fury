#version 330 core

out vec4 glColor;


in vec3 fragmentPosition;
in vec3 fragmentNormal;

uniform float time;
vec2 resolution = vec2(10, 10);

uniform vec3 cameraPosition;
uniform vec3 lightPosition = vec3(3, 4, 5);
vec3 lightColor = vec3(1, 1, 1);
vec3 objectColor = vec3(1, 1, 1);
vec3 ambientColor = vec3(1, 1, 1);
float ambientIntensity = 0.1;
float haloStrength = 0.6;
float shininess = 32;

void main() {


	vec3 norm = normalize(fragmentNormal);
    vec3 lightDir = normalize(lightPosition - fragmentPosition);

    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = ambientColor * ambientIntensity;


    vec3 viewDir = normalize(cameraPosition - fragmentPosition);


    float dot1 = dot(lightDir, norm);



    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = lightColor * spec;

    float dot2 = 1 - dot(viewDir, lightDir);


    vec3 halo = vec3(1) * max(log2(dot2  * haloStrength), 0.0) * haloStrength * 0.5;

    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 dis = vec3(1) * dot(viewDir, reflectDir) * sqrt(10 / distance(fragmentPosition, lightPosition)) * 0.01;
    vec3 result = (ambient + diffuse + specular + halo + dis) * objectColor;
    glColor = vec4(result, 1.0);


}