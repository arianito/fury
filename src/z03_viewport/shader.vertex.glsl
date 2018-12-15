#version 330 core

layout (location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec3 fragmentPosition;
out vec3 fragmentNormal;

void main(void) {
	gl_Position = projection * view * world * vec4(vertexPosition_modelspace, 1);

    fragmentPosition = vec3(world * vec4(vertexPosition_modelspace, 1));
    fragmentNormal = mat3(transpose(inverse(world))) * vertexNormal_modelspace;
}