#include "earthframe.h"

void EarthFrame::Init(CEngine* e) {
	engine = e;
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load shaders
	shader = new Shader("shaders/shader.vert", "shaders/shader.frag");
	lshader = new Shader("shaders/lamp.vert", "shaders/lamp.frag");

	// Create bodies
	earth = new Body("models/earth.obj", {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		6371000.0f, // 6.371 * 10^6 m
		5972000000000000000000000.0f // 5.972 * 10^24 kg
	});
	earth->SetScale(0.000000015f);

	moon = new Body("models/moon.obj", {
		glm::vec2(0.0f, 363300000.0f), // 3.633 * 10^8 m
		glm::vec2(1076.0f, 0.0f), // 1.076 * 10^3 m/s
		glm::vec2(0.0f, 0.0f),
		1737000.0f, // 1.737 * 10^6 m
		73476730900000000000000.0f // 7.3 * 10^22 kg
	});
	moon->SetScale(0.000000015f);

	// Load models
	lampModel = new Model("models/lamp.obj");

	// Set up point lights
	lights.push_back({ 
		glm::vec3(1.2f, 1.0f, 1.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(1.0f, 1.0f, 1.0f) });

	lights.push_back({
		glm::vec3(-2.0f, -2.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(1.0f, 1.0f, 1.0f) });

	// Set up rendering matrices
	proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	text = new TextRenderer();
	text->LoadFont("fonts/arial.ttf", 48);
}

void EarthFrame::Cleanup() {}
void EarthFrame::Pause() {}
void EarthFrame::Resume() {}

void EarthFrame::ProcessInput(bool* keyboard, double mxpos, double mypos) {
	// [ESC] -> Quit
	if (keyboard[GLFW_KEY_ESCAPE])
		engine->Quit();

	// [W][A][S][D] -> Strafe camera
	if (keyboard[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keyboard[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keyboard[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyboard[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (keyboard[GLFW_KEY_UP] && !up_held) {
		up_held = true;
		simSpeed *= 2;
		printf("Sim Speed: %f\n", simSpeed);
	}
	if (!keyboard[GLFW_KEY_UP] && up_held)
		up_held = false;
		
	if (keyboard[GLFW_KEY_DOWN] && !down_held) {
		down_held = true;
		simSpeed /= 2;
		printf("Sim Speed: %f\n", simSpeed);
	}
	if (!keyboard[GLFW_KEY_DOWN] && down_held)
		down_held = false;

	// Mouse X/Y -> pitch/yaw camera
	yaw += (float)((mxpos - lastX) * sensitivity);
	pitch += (float)((lastY - mypos) * sensitivity);
	lastX = mxpos;
	lastY = mypos;

	// Clamp pitch to -89 < pitch < 89
	if (pitch < -89.0f)
		pitch = -89.0f;
	if (pitch > 89.0f)
		pitch = 89.0f;

	// Calculate new camera front vector
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void EarthFrame::Loop() {
	// a = -((G * M) / r^2)
	moon->p.accel = -((G_CONST * earth->p.mass) / pow(glm::distance(earth->p.pos, moon->p.pos), 2.0f)) * glm::normalize(moon->p.pos - earth->p.pos);

	moon->Evolve(simSpeed);

	// Update the view matrix with the camera position
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void EarthFrame::Render() {
	// Clear the color and depth buffer
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set render matrices
	shader->Use();
	shader->uMatrix4("view", view);
	shader->uMatrix4("proj", proj);
	shader->uVector3("viewPos", cameraPos);
	shader->uPointLights(lights);
	
	// Draw the model
	earth->Draw(shader);
	moon->Draw(shader);

	text->DrawText("Test String.", 25.0f, 25.0f, 0.4f, glm::vec3(0.4f, 0.8f, 0.0f), "fonts/arial.ttf");

	// End scene
	glfwSwapBuffers(engine->wnd);
}
