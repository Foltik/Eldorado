#include "body.h"

Body::Body(const char * modelPath, const BodyProperties & props) {
	model = new Model(modelPath);
	p = props;
}

Body::~Body() {
	delete model;
}

void Body::SetScale(float scl) {
	scale = scl;
}

void Body::Evolve() {
	p.x += p.vx;
	p.y += p.vy;
}

void Body::Draw(Shader* shader) {
	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(p.x * scale, p.y * scale, 0)) * glm::scale(transform, glm::vec3(p.rad * scale));
	shader->uMatrix4("model", transform);

	model->Draw(shader);
}