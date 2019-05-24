#include "global.h"
#include "Input.h"
#include "MainScene.h"
#include "DebugTimer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetMainWindowText("MIDIProject2");
	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);
	SetGraphMode(FMX, FMY, 32);
	if (DxLib_Init() == -1)	return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	MainScene mainScene;
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		Input::update();
		DebugTimer debugTimer1("Update");
		if (mainScene.Update() == -1) break;;
		debugTimer1.Stop();
		DebugTimer debugTimer2("Draw");
		mainScene.Draw();
		debugTimer2.Stop();
		debugTimer1.Draw(FMX - 150, FMY - 40);
		debugTimer2.Draw(FMX - 150, FMY - 20);
	}
	DxLib_End();
	return 0;
}
