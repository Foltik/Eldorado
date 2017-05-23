#include "demoframe.h"

CDemoFrame CDemoFrame::pInstance;

void CDemoFrame::Init(CEngine* e) {
	engine = e;
	
	glEnable(GL_DEPTH_TEST);

	// Load shaders
	shader = new Shader("shaders/shader.vert", "shaders/shader.frag");
	lshader = new Shader("shaders/lamp.vert", "shaders/lamp.frag");

	// Create bodies
	earth = new Body("models/earth.obj", {
		glm::vec2(0.0f, 0.0f), // x, y in m/s
		glm::vec2(0.0f, 0.0f), // vx, vy in m/s
		glm::vec2(0.0f, 0.0f), // ax, ay in m/s
		6371000.0f, // 6.371 * 10^6 kg
		5972000000000000000000000.0f // 5.972 * 10^24 kg
	});
	earth->SetScale(0.000000015f);

	moon = new Body("models/moon.obj", {
		glm::vec2(0.0f, 363300000.0f), // 3.633 * 10^8 m
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		1737000.0f, // 1.737 * 10^6
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
	proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
}

void CDemoFrame::Cleanup() {}
void CDemoFrame::Pause() {}
void CDemoFrame::Resume() {}

void CDemoFrame::ProcessInput(bool* keyboard, double mxpos, double mypos) {
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

void CDemoFrame::Loop() {
	if (glm::distance(earth->p.pos, moon->p.pos) > earth->p.rad)
		moon->p.accel = -((G_CONST * earth->p.mass) / pow(glm::distance(earth->p.pos, moon->p.pos), 2.0f)) * glm::normalize(moon->p.pos - earth->p.pos);
	else
		moon->p.accel = glm::vec2(0.0f);
	
	printf("ACCEL: %f, %f\n", moon->p.accel.x, moon->p.accel.y);
	printf("VEL  : %f, %f\n", moon->p.vel.x, moon->p.vel.y);
	moon->Evolve();
	// Update the view matrix with the camera position
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void CDemoFrame::Render() {
	// Clear the color and depth buffer
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// *** Drawing the Model *** //
	shader->Use();
	
	// Set render matrices
	shader->uMatrix4("view", view);
	shader->uMatrix4("proj", proj);

	shader->uPointLights(lights);
	
	// Set parameters for shader
	shader->uVector3("viewPos", cameraPos);
	
	// Draw the model
	earth->Draw(shader);
	moon->Draw(shader);

	// *** Drawing the Lights *** //
	lshader->Use();

	lshader->uMatrix4("lview", view);
	lshader->uMatrix4("lproj", proj);

	// Set model matrix to proper position for drawing
	lmodel = glm::mat4();
	lmodel = glm::translate(lmodel, glm::vec3(1.2f, 1.0f, 1.5f)) * glm::scale(lmodel, glm::vec3(0.2f));
	lshader->uMatrix4("lmodel", lmodel);
	
	// Draw one light
	lampModel->Draw(lshader);

	// Translate to new light position
	lmodel = glm::mat4();
	lmodel = glm::translate(lmodel, glm::vec3(-2.0f, -2.0f, 0.0f)) * glm::scale(lmodel, glm::vec3(0.2f));
	lshader->uMatrix4("lmodel", lmodel);

	// Draw the final light
	lampModel->Draw(lshader);

	// End scene
	glfwSwapBuffers(engine->wnd);
}
