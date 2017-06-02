#include <k5/k5.h>

#include "menuframe.h"

int main() {
	CEngine& game = CEngine::Instance();

	game.CreateWindow("K5 Engine Test", 800, 600, false);

	game.ChangeFrame(&MenuFrame::Instance());

	while (game.Running()) 
		game.Tick();

	game.Cleanup();

	return K5_EXIT_SUCCESS;
}
