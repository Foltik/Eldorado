#include "menuframe.h"

void MenuFrame::Init(CEngine* e) {
	engine = e;

	shader = new Shader("shaders/menu.vert", "shaders/menu.frag");

	menuTextures = new Texture[3];
	menuTextures[0] = Texture("textures/menu0.png");
	menuTextures[1] = Texture("textures/menu1.png");
	menuTextures[2] = Texture("textures/menu2.png");

	GLfloat vertices[] = {
		-1.0f, -1.0f,  0.0f, 1.0f,
		-1.0f, 1.0f,   0.0f, 0.0f,
		 1.0f, 1.0f,   1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
}

void MenuFrame::Cleanup() {
	delete shader;
}
void MenuFrame::Pause() {}
void MenuFrame::Resume() {}

void MenuFrame::Loop() {}

void MenuFrame::ProcessInput(bool* keyboard, double mxpos, double mypos) {
	Rect button1 = {
		{ 303, 287 },
		{ 329, 243 },
		{ 543, 243 },
		{ 517, 287 }
	};

	Rect button2 = {
		{ 303, 353 },
		{ 329, 309 },
		{ 542, 309 },
		{ 517, 353 }
	};
}

void MenuFrame::Render() {
	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	shader->uInt("tex", 0);
	glBindTexture(GL_TEXTURE_2D, menuTextures[menuState].texture);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(engine->wnd);
}

bool MenuFrame::inRect(Rect r, double mxpos, double mypos) {

}