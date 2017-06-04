#include <k5/k5.h>
#include <exception>

#include "menuframe.h"

int main() {
	try {
		CEngine& game = CEngine::Instance();

		game.CreateWindow("K5 Engine Test", 800, 600, false);

		game.ChangeFrame(&MenuFrame::Instance());

		while (game.Running())
			game.Tick();

		game.Cleanup();
	}
	catch (std::exception* e) {
		printf("%s", e->what());
		return K5_EXIT_FAILURE;
	}

	return K5_EXIT_SUCCESS;
}
