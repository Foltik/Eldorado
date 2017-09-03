#include <k5/k5.h>

#include "menuframe.h"

int main(int argc, char* argv[]) {
	try {
		CEngine& game = CEngine::Instance();
		game.Init("K5 Engine TEst", 800, 600, argv[0]);
		game.ChangeFrame(&MenuFrame::Instance());
		game.Launch();
	}
	catch (std::exception* e) {
		printf("%s", e->what());
		return 1;
	}

	return 0;
}
