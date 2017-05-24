#include <k5/k5.h>

#include "demoframe.h"


int main() {

	CEngine::Instance().CreateWindow("K5 Engine Test", 800, 600, false);

	CEngine::Instance().ChangeFrame(CDemoFrame::Instance());

	while (CEngine::Instance().Running()) 
		CEngine::Instance().Tick();

	CEngine::Instance().Cleanup();

	return K5_EXIT_SUCCESS;
}
