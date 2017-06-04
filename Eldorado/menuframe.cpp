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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	text = new TextRenderer();
	text->LoadFont("fonts/Meslo.ttf", 48);
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
	if (keyboard[GLFW_KEY_ESCAPE])
		engine->Quit();

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

	auto inRect = [](Rect r, float mx, float my) {
		// Determines if the point (mx, my) is inside the Rect r
		my = CEngine::wndH - my;
		return (
			mx >= (my - (r.tl.y - (((r.tl.y - r.bl.y) / (r.tl.x - r.bl.x)) * r.tl.x))) / ((r.tl.y - r.bl.y) / (r.tl.x - r.bl.x)) &&
			mx <= (my - (r.tr.y - (((r.tr.y - r.br.y) / (r.tr.x - r.br.x)) * r.tr.x))) / ((r.tr.y - r.br.y) / (r.tr.x - r.br.x)) &&
			my >= r.bl.y &&
			my <= r.tl.y
			);
	};

	if (inRect(button1, (float)mxpos, (float)mypos))
		menuState = 1;
	else if (inRect(button2, (float)mxpos, (float)mypos))
		menuState = 2;
	else
		menuState = 0;

	if (menuState == 1 && mouse[GLFW_MOUSE_BUTTON_1])
		engine->ChangeFrame(&EarthFrame::Instance());

	if (menuState == 2 && mouse[GLFW_MOUSE_BUTTON_1])
		showInfo = true;
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
	glBindVertexArray(0);

	if (showInfo) {
		text->DrawText("Hold  [ i ] » Controls", 305.0f, 220.0f, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f), "fonts/Meslo.ttf");
		text->DrawText("Press [Esc] » Quit", 305.0f, 190.0f, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f), "fonts/Meslo.ttf");
	}

	glfwSwapBuffers(engine->wnd);
}
