#include <k5/k5.h>

#include "menuframe.h"

int main(int argc, char* argv[]) {
	try {
		CEngine& game = CEngine::Instance();

        game.SetCwd(argv[0]);

		game.CreateWindow("K5 Engine Test", 800, 600, false);

		game.ChangeFrame(&MenuFrame::Instance());

		while (game.Running())
			game.Tick();

		game.Cleanup();
	}
	catch (std::exception* e) {
		printf("%s", e->what());
		return 1;
	}

	return 0;
}
