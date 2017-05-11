#pragma once
#include <k5/k5.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>


class CDemoFrame : public CFrame{
public:
	virtual void Init() override;
	virtual void Cleanup() override;

	virtual void Pause() override;
	virtual void Resume() override;

	virtual void ProcessInput(bool* keyboard, double mxpos, double mypos) override;
	virtual void Loop() override;
	virtual void Render() override;

	static CDemoFrame* Instance() {
		return &pInstance;
	}

protected:
	CDemoFrame() = default;

private:
	static CDemoFrame pInstance;

	glm::mat4 lmodel;
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 1.5f);

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;

	float cameraSpeed = 0.03f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.5f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float pitch = 0.0f;
	float yaw = -90.0f;
	double lastX, lastY;
	double sensitivity = 0.020;
	
	Model* testModel;
	Model* lampModel;

	Shader* shader;
	Shader* lshader;
};
