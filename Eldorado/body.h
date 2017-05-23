#pragma once
#include <k5/k5.h>

struct BodyProperties {
	float x, y;
	float vx, vy;
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
private:
	Model* model;
	glm::mat4 transform;
	
	BodyProperties p;
	float scale = 1.0f;
};