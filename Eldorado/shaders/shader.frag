#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D tex;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
  
void main() {
	float ambientImpact = 0.1f;
	vec4 ambient = vec4(ambientImpact * lightColor, 1.0f);

	vec3 lightDir = normalize(lightPos - FragPos);
	float diffImpact = max(dot(Normal, lightDir), 0.0);
	vec4 diffuse = vec4(diffImpact * lightColor, 1.0f);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float specularStrength = 1.0f;
	float specImpact = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;
	vec4 specular = vec4(specImpact * lightColor, 1.0f);

	color = (ambient + diffuse + specular) * texture(tex, TexCoord);
}
