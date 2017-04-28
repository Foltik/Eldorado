#include "demoframe.h"

CDemoFrame CDemoFrame::pInstance;

void CDemoFrame::Init() {
	vertex3f data[6] = { vec3f(-0.5f, -0.5f, 0.f), vec3f(1.0f, 0.0f, 0.0f), vec3f(0.f, 0.5f, 0.f), vec3f(0.0f, 1.0f, 0.0f), vec3f(0.5f, -0.5f, 0.f), vec3f(0.0f, 0.0f, 1.0f) };
	GLuint vbo;

	shader = new Shader("./shaders/shader.vert", "./shaders/shader.frag");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CDemoFrame::Cleanup() {

}

void CDemoFrame::Pause() {}
void CDemoFrame::Resume() {}

void CDemoFrame::PollEvents() {
	glfwPollEvents();
}

void CDemoFrame::Loop() {

}

void CDemoFrame::Render() {
	glClearColor(0.6f, 0.75f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shader->program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	
	glfwSwapBuffers(engine->wnd);
}
