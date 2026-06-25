#include <windows.h>
#include "../include/game.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Game game;
	game.launch();
	return 0;
}