#include <k5/k5.h>

#include "demoframe.h"


int main() {
	CEngine& game = CEngine::Instance();

	game.CreateWindow("K5 Engine Test", 800, 600, false);

	game.ChangeFrame(CDemoFrame::Instance());

	while (game.Running()) 
		game.Tick();

	game.Cleanup();

	return K5_EXIT_SUCCESS;
}
