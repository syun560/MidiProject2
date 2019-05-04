#include "MainScene.h"
#include "Input.h"

MainScene::MainScene() {
	font = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
}

void MainScene::Update() {
	// 操作系
	if (Input::Key(KEY_INPUT_O) == 1) { // ファイル読み込み
		if (smfio.read("input.mid") == -1) printfDx("読み込み失敗\n");
		else printfDx("読み込み成功\n");
	}
	if (Input::Key(KEY_INPUT_S) == 1) { // ファイル書き出し
		char data[10000]; // TODO できれば動的に決定すべき
		int size = midiEventManager.getMidiMsgForSMF(data);
		if (smfio.write("output.mid", data, size) == -1) printfDx("書き込み失敗\n");
		else printfDx("書き込み成功\n");
	}
	if (Input::Key(KEY_INPUT_A) == 1) { // 再生
		PlayMusic("output.mid", DX_PLAYTYPE_BACK);
		printfDx("%sを再生\n", "output.mid");
	}
	if (Input::Key(KEY_INPUT_J) == 1) { // 自動作曲
		midiEventManager.autoCreate(480 * 3 * 32);
	}

	if (Input::Key(KEY_INPUT_D) == 1) StopMusic(); // 停止

	// デバッグ系
	if (Input::Key(KEY_INPUT_C) == 1) {
		clsDx();
		midiEventManager.deleteAllEvent();
	}

	midiController.Update(conductor.Update());
}

void MainScene::Draw(){
	// MIDIイベントを表示
	midiEventManager.draw();
	midiController.Draw();

	// 操作方法を表示
	if (Input::Key(KEY_INPUT_M) > 0) {
		static const int INFO_X = 400;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(INFO_X - 20, 80, FMX - 20, 240, BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawFormatString(INFO_X, 80, WHITE, "操作方法\nOキーで読込\nSキーで保存\nAキーで再生\nDキーで停止\nCキーでクリア\nESCで終了");
	}
	

	// TODO:テンポやプレイ状況、スケールを表示
	// コードを表示
	DrawStringToHandle(FMX/2 - 30, FMY/2 - 30, "Dm", WHITE, font);
}