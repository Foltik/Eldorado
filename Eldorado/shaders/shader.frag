#version 330 core
struct Material {
    sampler2D diffuse;
	sampler2D specular;
    float shine;
};

struct PointLight {
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_POINT_LIGHTS 4

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Material material;
uniform PointLight pLights[MAX_POINT_LIGHTS];

uniform sampler2D tex_diffuse1;
uniform sampler2D tex_diffuse2;
uniform sampler2D tex_diffuse3;

uniform sampler2D tex_specular1;
uniform sampler2D tex_specular2;
uniform sampler2D tex_specular3;

uniform vec3 viewPos;

vec3 ProcessPointLights();
  
void main() {
	vec3 result = ProcessPointLights();
	color = color = vec4(result, 1.0f);
}

vec3 ProcessPointLights() {
	vec3 result = vec3(0.0f);
	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 lightDir = normalize(pLights[i].pos - FragPos);
		vec3 reflectDir = reflect(-lightDir, Normal);

		vec3 ambient = pLights[i].ambient * vec3(texture(material.diffuse, TexCoord));

		float diffAngleFactor = max(dot(Normal, lightDir), 0.0);
		vec3 diffuse = pLights[i].diffuse * diffAngleFactor * vec3(texture(material.diffuse, TexCoord));

		float specAngleFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
		vec3 specular = pLights[i].specular * specAngleFactor * vec3(texture(material.specular, TexCoord));

		result += (ambient + diffuse + specular);
	}
	return result;
}