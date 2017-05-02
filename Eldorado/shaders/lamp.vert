#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 lmodel;
uniform mat4 lview;
uniform mat4 lproj;

void main() {
    gl_Position = lproj * lview * lmodel * vec4(position, 1.0);
}