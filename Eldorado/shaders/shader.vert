#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(position, 1.0);
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = normalize(mat3(transpose(inverse(model))) * normal);
}