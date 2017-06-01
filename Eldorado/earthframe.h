#pragma once
#include <k5/k5.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "body.h"

class EarthFrame : public IFrame {
public:
	virtual void Init(CEngine* e) override;
	virtual void Cleanup() override;

	virtual void Pause() override;
	virtual void Resume() override;

	virtual void ProcessInput(bool* keyboard, double mxpos, double mypos) override;
	virtual void Loop() override;
	virtual void Render() override;

	static EarthFrame& Instance() {
		static EarthFrame instance;
		return instance;
	}

protected:
	EarthFrame() = default;
	EarthFrame(const EarthFrame&) = delete;
	EarthFrame(EarthFrame&&) = delete;
	void operator=(const EarthFrame&) = delete;
	void operator=(EarthFrame&&) = delete;

private:
	static constexpr float G_CONST = 0.0000000000667300f; // 6.67300 * 10^-11 * m^3 * kg^-1 * s^-2

	static EarthFrame pInstance;

	float simSpeed = 1.0f;

	float cameraSpeed = 0.05f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.5f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float pitch = 0.0f;
	float yaw = -90.0f;
	double lastX, lastY;
	double sensitivity = 0.020;

	bool up_held;
	bool down_held;
	
	Body* earth;
	Body* moon;

	glm::mat4 view;
	glm::mat4 proj;

	glm::mat4 lmodel;
	std::vector<PointLight> lights;

	Model* lampModel;
	Shader* shader;
	Shader* lshader;

	TextRenderer* text;
};
