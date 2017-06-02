#version 330 core
in vec2 texCoord;
out vec4 color;

uniform sampler2D tex;

void main() {
	color = vec4(vec3(texture(tex, texCoord)), 1.0);
}