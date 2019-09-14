#define _CRT_SECURE_NO_WARNINGS
#include "MainScene.h"
#include "Input.h"

// 五度圏
static const char CIRCLE_OF_FIFTH[2][12][5] = {
	"C",  "G",  "D",  "A",    "E",    "B",   "G♭",  "D♭",  "A♭", "E♭", "B♭", "F",
	"Am", "Em", "Bm", "F♯m", "C♯m", "G#m", "D♯m", "B♭m", "Fm",  "Cm",  "Gm",  "Dm"
};

static const char CHORD_NAME[2][12][5] = {
	"C", "D♭", "D",  "E♭", "E",  "F",  "G♭", "G",  "A♭", "A",  "B♭", "B",
	"C", "C#m", "Dm", "D#m", "Em", "Fm", "F#m", "Gm", "G#m", "Am", "B♭m", "Bm",
};

MainScene::MainScene() {
	font = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
	if(midiController.Init() == -1) printfDx("MIDI初期化失敗\n");
	InputMode = false;
	legacyMode = false; // 昔の機能
	key1 = key2 = 0;
	rootNote = 60;
	baseNote = 0;
}

int MainScene::ExecCommand(const char* command) {
	if (strcmp(command, "exit") == 0) return -1;
	if (strcmp(command, "open") == 0) {
		FileOpen("data.dat");
	}
	if (strcmp(command, "save") == 0) {
		FileOpen("data.dat");
	}
	return 0;
}

void MainScene::FileOpen(const char* filename) {
	FILE* fp;
	sprintf_s(FilePath, 100, "data/%s", filename);
	if (fopen_s(&fp, FilePath, "wb") == 0) {
		gridRoll.Save(fp);
		midiController.Save(fp);
		conductor.Save(fp);
		fclose(fp);
	}
}

void MainScene::FileSave(const char* filename) {
	FILE* fp;
	sprintf_s(FilePath, 100, "data/%s", filename);
	if (fopen_s(&fp, FilePath, "rb") == 0) {
		gridRoll.Load(fp);
		midiController.Load(fp);
		conductor.Load(fp);
		fclose(fp);
	}
}

int MainScene::tenkai(int key) {
	int res = rootNote + baseNote + key;
	while (res >= rootNote + 12) res -= 12;
	return res;
}

int MainScene::Update() {
	// 更新
	// ここでtickの値を得る
	int c = conductor.Update();
	midiController.Update(conductor.GetDelta());
	// TODO 同じような処理が重複してる
	gridRoll.Update(conductor.GetMea(), midiController.GetFocusCh());
	midiEventManager.Update(midiController.GetFocusCh(), conductor.GetMea());

	// シーケンス
	if (c != -1) { // PLAYINGだったら
		for (int ch = 0; ch < 16; ch++) {
			key1 = (int)conductor.GetPreTick();
			key2 = (int)c;
			for (int key = key1; key < key2; key++) {
				// TODO 下の３つの関数を構造体などを使用してひとつにまとめたい、mapのlower関数を使いたい
				int note = midiEventManager.GetNoteData(ch, key);
				int gate = midiEventManager.GetGateData(ch, key);
				int vel = midiEventManager.GetVelData(ch, key);
				if (note == -1 || gate == -1 || vel == -1) continue;
				// printfDx("note: %d gate: %d\n", note, gate);
				midiController.Play(ch, note, gate, vel);
			}
		}
	}

	// 操作系
	static int holdtime = 30;
	if (InputMode) {
		const char* message = commandInput.Update();
		if (message != NULL) {
			if (ExecCommand(message) == -1) return -1;
			InputMode = false;
		}
	}
	else {
		if (Input::Key(KEY_INPUT_Q) == 1) {
			commandInput.TurnInputMode();
			InputMode = true;
		}
		else if (Input::Key(KEY_INPUT_LALT) > 0 || Input::Key(KEY_INPUT_RALT) > 0) {
			if (Input::Key(KEY_INPUT_LEFT) == 1 || Input::Key(KEY_INPUT_LEFT) > holdtime) conductor.Slower();
			if (Input::Key(KEY_INPUT_RIGHT) == 1 || Input::Key(KEY_INPUT_RIGHT) > holdtime) conductor.Faster();
		}
		else if (Input::Key(KEY_INPUT_LCONTROL) > 0 || Input::Key(KEY_INPUT_RCONTROL) > 0) {
			if (Input::Key(KEY_INPUT_RIGHT) == 1) conductor.NextMea();
			else if (Input::Key(KEY_INPUT_LEFT) == 1) {
				conductor.PreMea();
				if (Input::Key(KEY_INPUT_LSHIFT) > 0) while (conductor.PreMea());
			}
			else if (Input::Key(KEY_INPUT_UP) == 1) midiEventManager.keyUp();
			else if (Input::Key(KEY_INPUT_DOWN) == 1) midiEventManager.keyDown();
			else if (Input::Key(KEY_INPUT_D) == 1) gridRoll.DeleteAll();
		}
		else if (Input::Key(KEY_INPUT_LSHIFT) > 0) {
			if (Input::Key(KEY_INPUT_UP) == 1 || Input::Key(KEY_INPUT_UP) > holdtime) midiController.NextCh();
			else if (Input::Key(KEY_INPUT_DOWN) == 1 || Input::Key(KEY_INPUT_DOWN) > holdtime) midiController.PreCh();
			else if (Input::Key(KEY_INPUT_LEFT) == 1 || Input::Key(KEY_INPUT_LEFT) > holdtime) midiController.PreProgram();
			else if (Input::Key(KEY_INPUT_RIGHT) == 1 || Input::Key(KEY_INPUT_RIGHT) > holdtime) midiController.NextProgram();
		}
		else {
			if (Input::Key(KEY_INPUT_S) == 1) {
				// FileOpen("data.dat");
				unsigned char data[10000]; // TODO できれば動的に決定すべき
				int size = midiEventManager.getMidiMsgForSMF(data);
				if (smfio.write("output.mid", data, size) == -1) printfDx("書き込み失敗\n");
				else printfDx("書き込み成功\n");
			}
			else if (Input::Key(KEY_INPUT_O) == 1) { // ファイル読み込み
				// FileSave("data.dat");
				if (smfio.read("kaeru.mid") == -1) printfDx("読み込み失敗\n");
				else {
					for (int i = 0; i < smfio.getTrackNum(); ++i) {
						double bpm = midiEventManager.loadMidiMsgFromSMF(i, smfio.getTrackData(i), smfio.getTrackSize(i));
						if (bpm != 0.0) conductor.SetTempo(bpm);
					}
					printfDx("読み込み成功\n");
					smfio.draw();
				}
			}
			else if (Input::Key(KEY_INPUT_A) == 1) { // 再生
				if (PlayMusic("output.mid", DX_PLAYTYPE_BACK) == -1) printfDx("エラー発生\n");
				else printfDx("%sを再生\n", "output.mid");
			}
			else if (Input::Key(KEY_INPUT_RETURN) == 1) { // 自動作曲
				midiEventManager.deleteAllEvent();
				midiEventManager.autoCreate(480 * 3 * 32);
			}
			else if (Input::Key(KEY_INPUT_Z) == 1) { // 単音を鳴らす
				midiController.Play(midiController.GetFocusCh(), 60, 480 * 2, 100);
			}
			else if (Input::Key(KEY_INPUT_X) == 1) StopMusic(); // MIDI停止
			else if (Input::Key(KEY_INPUT_D) == 1) gridRoll.DeleteOnePhrase();
			else if (Input::Key(KEY_INPUT_C) == 1) gridRoll.Copy();
			else if (Input::Key(KEY_INPUT_V) == 1) gridRoll.Paste();
			else if (Input::Key(KEY_INPUT_UP) == 1) midiEventManager.HigherOctave();
			else if (Input::Key(KEY_INPUT_DOWN) == 1) midiEventManager.LowerOctave();
			else if (Input::Key(KEY_INPUT_RIGHT) == 1) {
				midiController.AllStop();
				conductor.EndBeat();
			}
			else if (Input::Key(KEY_INPUT_LEFT) == 1) {
				midiController.AllStop();
				conductor.FirstBeat();
			}
			else if (Input::Key(KEY_INPUT_R) == 1) conductor.SetRepeat();
			else if (Input::Key(KEY_INPUT_BACK) == 1) {
				midiEventManager.deleteAllEvent();
			}
			else if (Input::Key(KEY_INPUT_DELETE) == 1) {
				clsDx();
			}
		}
		if (Input::Key(KEY_INPUT_NUMPAD7) == 1) { // 転調♭
				baseNote += 5;
				baseNote %= 12;
				midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480 * 2, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD8) == 1) { // ナチュラル
			baseNote = 0;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480 * 2, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD9) == 1) { // 転調♯
			baseNote += 7;
			baseNote %= 12;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480 * 2, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD4) == 1) { // �Wの和音
			if (Input::Key(KEY_INPUT_LSHIFT) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(5), 480 * 2, 100, false, true);
			else midiController.PlayChord(midiController.GetFocusCh(), tenkai(5), 480 * 2, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD5) == 1) { // �Tの和音
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480 * 2, 100);
			if (Input::Key(KEY_INPUT_LSHIFT) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480 * 2, 100, false, true);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD6) == 1) { // �Xの和音
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(7), 480 * 2, 100);
			if (Input::Key(KEY_INPUT_LSHIFT) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480 * 2, 100, false, true);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD1) == 1) { // �Umの和音
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(2), 480 * 2, 100, true);
			if (Input::Key(KEY_INPUT_LSHIFT) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(2), 480 * 2, 100, true, true);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD2) == 1) { // �Ymの和音
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(9), 480 * 2, 100, true);
			if (Input::Key(KEY_INPUT_LSHIFT) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(2), 480 * 2, 100, true, true);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD3) == 1) { // �Vmの和音
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(4), 480 * 2, 100, true);
			if (Input::Key(KEY_INPUT_LSHIFT) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(2), 480 * 2, 100, true, true);
		}
		if (Input::Key(KEY_INPUT_SPACE) == 1) {
			midiController.AllStop();
			conductor.PlayOrPause();
		}
	}
	return 0;
}

void MainScene::Draw(){
	// MIDIイベントを表示
	midiEventManager.draw();
	midiController.Draw();
	conductor.Draw(50, 50);
	gridRoll.Draw();
	commandInput.Draw(0, FMY - 20);

	// 操作方法を表示
	if (Input::Key(KEY_INPUT_M) > 0) {
		static const int INFO_X = 400;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(INFO_X - 20, 80, FMX - 20, 240, BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawFormatString(INFO_X, 80, WHITE, "操作方法\nOキーで読込\nSキーで保存\nAキーで再生\nDキーで停止\nCキーでクリア\nESCで終了");
	}

	// コードを表示
	static const int CHORD_X = 400, CHORD_Y = 400;
	static const int MARGIN = 40;
	DrawString(CHORD_X - MARGIN, CHORD_Y         , CHORD_NAME[0][(baseNote + 5) % 12], WHITE);
	DrawString(CHORD_X         , CHORD_Y         , CHORD_NAME[0][(baseNote + 0) % 12] , WHITE);
	DrawString(CHORD_X + MARGIN, CHORD_Y         , CHORD_NAME[0][(baseNote + 7) % 12], WHITE);
	DrawString(CHORD_X - MARGIN, CHORD_Y + MARGIN, CHORD_NAME[1][(baseNote + 2) % 12], WHITE);
	DrawString(CHORD_X         , CHORD_Y + MARGIN, CHORD_NAME[1][(baseNote + 9) % 12], WHITE);
	DrawString(CHORD_X + MARGIN, CHORD_Y + MARGIN, CHORD_NAME[1][(baseNote + 4) % 12], WHITE);

	// TODO:テンポやプレイ状況、スケールを表示
	// コードを表示
	if (legacyMode)
		DrawStringToHandle(FMX/2 - 30, FMY/2 - 30, "Dm", WHITE, font);
}