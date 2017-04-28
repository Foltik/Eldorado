#pragma once
#include <k5/k5.h>

#include <GL/glew.h>


class CDemoFrame : public CFrame{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void PollEvents();
	void Loop();
	void Render();

	static CDemoFrame* Instance() {
		return &pInstance;
	}

protected:
	CDemoFrame() {}

private:
	static CDemoFrame pInstance;

	Shader* shader;
	GLuint vao;

	int initGraphics = 0;
};
