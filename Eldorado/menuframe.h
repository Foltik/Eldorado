#pragma once
#include <k5/k5.h>

struct Point {
	int x;
	int y;
};

struct Rect {
	Point bl;
	Point tl;
	Point tr;
	Point br;
};

class MenuFrame : public IFrame {

public:
	virtual void Init(CEngine* e) override;
	virtual void Cleanup() override;

	virtual void Pause() override;
	virtual void Resume() override;

	virtual void ProcessInput(bool* keyboard, double mxpos, double mypos) override;
	virtual void Loop() override;
	virtual void Render() override;

	static MenuFrame& Instance() {
		static MenuFrame instance;
		return instance;
	}

protected:
	MenuFrame() = default;
	MenuFrame(const MenuFrame&) = delete;
	MenuFrame(MenuFrame&&) = delete;
	void operator=(const MenuFrame&) = delete;
	void operator=(MenuFrame&&) = delete;

private:
	bool inRect(Rect r, double mxpos, double mypos);

	int menuState = 0;

	Texture* menuTextures;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	Shader* shader;

	CEngine* engine;
};