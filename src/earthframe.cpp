#include "earthframe.h"

void EarthFrame::Init(CEngine* e) {
	engine = e;

	engine->disableCursor();

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
	earth->SetScale(0.00000004f);

	moon = new Body("models/moon.obj", {
		glm::vec2(0.0f, 363300000.0f), // 3.633 * 10^8 m
		glm::vec2(1076.0f, 0.0f), // 1.076 * 10^3 m/s
		glm::vec2(0.0f, 0.0f),
		1737000.0f, // 1.737 * 10^6 m
		73476730900000000000000.0f // 7.3 * 10^22 kg
	});
	moon->SetScale(0.00000004f);

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
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	text = new TextRenderer();
	text->LoadFont("fonts/Meslo.ttf", 48);
}

void EarthFrame::Cleanup() {}
void EarthFrame::Pause() {}
void EarthFrame::Resume() {}

void EarthFrame::ProcessInput(bool* keyboard, bool* mouse, double mxpos, double mypos) {
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

	drawControls = keyboard[GLFW_KEY_I];

	if (keyboard[GLFW_KEY_RIGHT] && !right_held) {
		right_held = true;
		if (simSpeed < 262144.0f)
			simSpeed *= 2;
	}
	if (!keyboard[GLFW_KEY_RIGHT] && right_held)
		right_held = false;
		

	if (keyboard[GLFW_KEY_LEFT] && !left_held) {
		left_held = true;
		if (simSpeed > 1.0f)
			simSpeed /= 2;
	}
	if (!keyboard[GLFW_KEY_LEFT] && left_held)
		left_held = false;


	if (keyboard[GLFW_KEY_UP] && !up_held) {
		up_held = true;
		if (fov > 44.05f)
			fov -= 0.05f;
	}
	if (!keyboard[GLFW_KEY_UP] && up_held)
		up_held = false;


	if (keyboard[GLFW_KEY_DOWN] && !down_held) {
		down_held = true;
		if (fov < 45.95f)
			fov += 0.05f;
	}
	if (!keyboard[GLFW_KEY_DOWN] && down_held)
		down_held = false;


	if (keyboard[GLFW_KEY_R] && !r_held) {
		r_held = true;
		Reset();
	}
	if (!keyboard[GLFW_KEY_R] && r_held)
		r_held = false;

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
	proj = glm::perspective(fov, 800.0f / 600.0f, 0.1f, 1000.0f);
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

	if (drawControls) {
		text->DrawText("[  Up/Down   ]  >>  Zoom In/Out", 100.0f, 80.0f, 0.4f, glm::vec3(1.0f), "Meslo.ttf");
		text->DrawText("[ Left/Right ]  >>  Decrease/Increase Sim Speed", 100.0f, 110.0f, 0.4f, glm::vec3(1.0f), "Meslo.ttf");
		text->DrawText("     [ R ]      >>  Reset Simulation", 100.0f, 140.0f, 0.4f, glm::vec3(1.0f), "Meslo.ttf");
	}

	char cameraInfo[256];
	snprintf(cameraInfo, sizeof(cameraInfo), 
		"View Position: (%.1f, %.1f, %.1f) | View Angle: (%.2f, %.2f) | FoV: %.2f deg",
		cameraPos.x, cameraPos.y, cameraPos.z,
		pitch, yaw,
		fov);
	text->DrawText(std::string(cameraInfo), 10.0f, 10.0f, 0.3f, glm::vec3(0.0f, 1.0f, 0.0f), "Meslo.ttf");

	char simInfo[256];
	snprintf(simInfo, sizeof(simInfo),
		"Simulation Scale: [4.0E-8] | Simulation Speed: [%-8.1f]x", simSpeed);
	text->DrawText(std::string(simInfo) + (simSpeed == 1.0f ? " (Real Time)" : ""), 10.0f, 30.0f, 0.3f, glm::vec3(0.0f, 1.0f, 0.0f), "Meslo.ttf");

	char moonInfo[256];
	snprintf(moonInfo, sizeof(moonInfo),
		"Orbital Velocity: %-6.2f m/s | Orbital Distance: %.3fE+8 m",
		glm::length(moon->p.vel),
		glm::distance(earth->p.pos, moon->p.pos) / (float)100000000);
	text->DrawText(std::string(moonInfo), 10.0f, 50.0f, 0.3f, glm::vec3(0.0f, 1.0f, 0.0f), "Meslo.ttf");
}

void EarthFrame::Reset() {
	cameraPos = glm::vec3(-0.1f, 0.0f, -14.8f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	fov = 45.0f;
	pitch = 0.0f;
	yaw = -280.0f;

	simSpeed = 1.0f;

	moon->p.pos = glm::vec2(0.0f, 363300000.0f);
	moon->p.vel = glm::vec2(1076.0f, 0.0f);
}
