#pragma once
#include <k5/k5.h>

struct BodyProperties {
	glm::vec2 pos;
	glm::vec2 vel;
	glm::vec2 accel;
	float rad;
	float mass;
};

class Body {
public:
	Body(const char* modelPath, const BodyProperties& props);
	~Body();

	void SetScale(float scl);

	void Evolve();
	void Draw(Shader* shader);

	BodyProperties p;
private:
	Model* model;
	glm::mat4 transform;
	
	float scale = 1.0f;
};
