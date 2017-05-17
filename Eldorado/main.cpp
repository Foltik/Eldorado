#include <k5/k5.h>

#include "demoframe.h"


int main() {
	CEngine game("K5 Engine Test", 800, 600, false);

	// Initialize Engine
	if (!game.Init()) return K5_EXIT_FAILURE;

	// Set Initial Frame
	game.ChangeFrame(CDemoFrame::Instance());

	while (game.Running()) 
		game.Tick();

	game.Cleanup();

	return K5_EXIT_SUCCESS;
}
