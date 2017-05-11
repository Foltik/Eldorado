#include "demoframe.h"

CDemoFrame CDemoFrame::pInstance;

void CDemoFrame::Init() {
	glEnable(GL_DEPTH_TEST);

	// Load shaders
	shader = new Shader("./shaders/shader.vert", "./shaders/shader.frag");
	lshader = new Shader("./shaders/lamp.vert", "./shaders/lamp.frag");

	// Load model
	testModel = new Model("models/HUMAN.blend");
	
	GLfloat lamp[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	// Generate buffers and store data for lamp
	glGenVertexArrays(1, &lvao);
	glGenBuffers(1, &lvbo);

	glBindVertexArray(lvao);
	glBindBuffer(GL_ARRAY_BUFFER, lvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lamp), lamp, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	// Set up rendering matrices
	proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	// Translate light model matrix to the proper position and scale it down
	lmodel = glm::translate(lmodel, lightPos) * glm::scale(lmodel, glm::vec3(0.2f));
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
	// Rotate the model every tick
	model = glm::rotate(model, 0.005f, glm::vec3(1.0f, 0.3f, 0.5f));
	
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
	shader->uMatrix4("model", glm::scale(model, glm::vec3(0.5f)));
	shader->uMatrix4("view", view);
	shader->uMatrix4("proj", proj);
	
	// Set parameters for shader
	shader->uVector3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->uVector3("lightPos", lightPos);
	shader->uVector3("viewPos", cameraPos);
	
	// Draw the model
	testModel->Draw(shader);



	// *** Drawing the Light *** //
	lshader->Use();

	lshader->uMatrix4("lmodel", lmodel);
	lshader->uMatrix4("lview", view);
	lshader->uMatrix4("lproj", proj);

	// Draw the light
	glBindVertexArray(lvao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);



	// End scene
	glfwSwapBuffers(engine->wnd);
}
