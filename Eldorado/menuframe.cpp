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
void MenuFrame::Resume() {
	glfwSetInputMode(engine->wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void MenuFrame::Loop() {}

void MenuFrame::ProcessInput(bool* keyboard, bool* mouse, double mxpos, double mypos) {
	Rect button1 = {
		{ 305.0f, 312.0f },
		{ 330.0f, 355.0f },
		{ 543.0f, 355.0f },
		{ 517.0f, 312.0f }
	};

	Rect button2 = {
		{ 305.0f, 246.0f },
		{ 330.0f, 289.0f },
		{ 543.0f, 289.0f },
		{ 517.0f, 246.0f }
	};

	if (inRect(button1, mxpos, mypos))
		menuState = 1;
	else if (inRect(button2, mxpos, mypos))
		menuState = 2;
	else
		menuState = 0;

	if (menuState == 1 && mouse[GLFW_MOUSE_BUTTON_1])
		engine->PushFrame(&EarthFrame::Instance());
}

void MenuFrame::Render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	shader->uInt("tex", 0);
	glBindTexture(GL_TEXTURE_2D, menuTextures[menuState].texture);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(engine->wnd);
}

bool MenuFrame::inRect(const Rect& r, double mxpos, double mypos) {
	float mx = (float)mxpos;
	float my = CEngine::wndH - (float)mypos;
	
	float ml = (r.tl.y - r.bl.y) / (r.tl.x - r.bl.x);
	float mr = (r.tr.y - r.br.y) / (r.tr.x - r.br.x);

	float il = r.tl.y - (ml * r.tl.x);
	float ir = r.tr.y - (mr * r.tr.x);
	
	return (mx >= ((my - il) / ml) &&
		   	mx <= ((my - ir) / mr) &&
			my >= r.bl.y &&
			my <= r.tl.y);
}
