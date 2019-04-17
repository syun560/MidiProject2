#include "global.h"
#include "Input.h"
#include "MainScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetMainWindowText("MIDIProject");
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)	return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	MainScene mainScene;
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		Input::update();
		mainScene.Update();
		mainScene.Draw();
		if (Input::Key(KEY_INPUT_ESCAPE) == 1) break;
	}
	DxLib_End();
	return 0;
}
