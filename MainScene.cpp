#define _CRT_SECURE_NO_WARNINGS
#include "MainScene.h"
#include "Input.h"

// 五度圏
static const char CIRCLE_OF_FIFTH[2][12][5] = {
	"C",  "G",  "D",  "A",    "E",    "B",   "G♭",  "D♭",  "A♭", "E♭", "B♭", "F",
	"Am", "Em", "Bm", "F♯m", "C♯m", "G#m", "D♯m", "B♭m", "Fm",  "Cm",  "Gm",  "Dm"
};

static const int MAJOR_SCALE[8] = {
	0, 0, 2, 4, 5, 7, 9, 11
};

static const char CHORD_NAME[6][12][8] = {
	"C", "D♭", "D",  "E♭", "E",  "F",  "G♭", "G",  "A♭", "A",  "B♭", "B",
	"Cm", "C#m", "Dm", "D#m", "Em", "Fm", "F#m", "Gm", "G#m", "Am", "B♭m", "Bm",
	"C7", "D♭7", "D7",  "E♭7", "E7",  "F7",  "G♭7", "G7",  "A♭7", "A7",  "B♭7", "B7",
	"Cm7", "C#m7", "Dm7", "D#m7", "Em7", "Fm7", "F#m7", "Gm7", "G#m7", "Am7", "B♭m7", "Bm7",
	"CM7", "D♭M7", "DM7",  "E♭M7", "EM7",  "FM7",  "G♭M7", "GM7",  "A♭M7", "AM7",  "B♭M7", "BM7",
	"CmM7", "C#mM7", "DmM7", "D#mM7", "EmM7", "FmM7", "F#mM7", "GmM7", "G#mM7", "AmM7", "B♭mM7", "BmM7",
};

static const char COMMON_CHORD[2][7][8] = {
	"C", "Dm", "Em",  "F", "G", "Am",
	"CM7", "Dm7", "Em7",  "FM7", "G7", "Am7"
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
	// 操作系
	static const int KENBAN_MAX = 18, CHORD_MAX = 10;
	int kenban[KENBAN_MAX] = {
		Input::Key(KEY_INPUT_Z),
		Input::Key(KEY_INPUT_S),
		Input::Key(KEY_INPUT_X),
		Input::Key(KEY_INPUT_D),
		Input::Key(KEY_INPUT_C),
		Input::Key(KEY_INPUT_V),
		Input::Key(KEY_INPUT_G),
		Input::Key(KEY_INPUT_B),
		Input::Key(KEY_INPUT_H),
		Input::Key(KEY_INPUT_N),
		Input::Key(KEY_INPUT_J),
		Input::Key(KEY_INPUT_M),
		Input::Key(KEY_INPUT_COMMA),
		Input::Key(KEY_INPUT_L),
		Input::Key(KEY_INPUT_PERIOD),
		Input::Key(KEY_INPUT_SEMICOLON),
		Input::Key(KEY_INPUT_SLASH),
		Input::Key(KEY_INPUT_BACKSLASH)
	};
	int numPad[CHORD_MAX] = {
		Input::Key(KEY_INPUT_NUMPAD0),
		Input::Key(KEY_INPUT_NUMPAD1),
		Input::Key(KEY_INPUT_NUMPAD2),
		Input::Key(KEY_INPUT_NUMPAD3),
		Input::Key(KEY_INPUT_NUMPAD4),
		Input::Key(KEY_INPUT_NUMPAD5),
		Input::Key(KEY_INPUT_NUMPAD6),
		Input::Key(KEY_INPUT_NUMPAD7),
		Input::Key(KEY_INPUT_NUMPAD8),
		Input::Key(KEY_INPUT_NUMPAD9),
	};
	int chordType = 0;
	if (Input::Key(KEY_INPUT_LCONTROL) > 0) chordType = 1;

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
			else if (Input::Key(KEY_INPUT_UP) == 1) {
				midiEventManager.keyUp();
				if(rootNote < 128 - 12) rootNote += 12;
			}
			else if (Input::Key(KEY_INPUT_DOWN) == 1) {
				midiEventManager.keyDown();
				if(rootNote >= 12) rootNote -= 12;
			}
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
			else if (Input::Key(KEY_INPUT_V) == 1) gridRoll.Paste();
			//else if (Input::Key(KEY_INPUT_X) == 1) StopMusic(); // MIDI停止
			//else if (Input::Key(KEY_INPUT_D) == 1) gridRoll.DeleteOnePhrase();
			//else if (Input::Key(KEY_INPUT_C) == 1) gridRoll.Copy();
			//else if (Input::Key(KEY_INPUT_V) == 1) gridRoll.Paste();
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
		for (int i = 0; i < KENBAN_MAX; i++) {
			if (kenban[i] > 0) midiController.PlayHold(midiController.GetFocusCh(), rootNote + baseNote + i, 100);
		}

		// Play Diatonic Chord
		static int chordCh = 1;
		for (int i = 1; i <= 6; i++) {
			if (numPad[i] > 0) {
				int note = rootNote + baseNote + MAJOR_SCALE[i];
				// root
				midiController.PlayHold(chordCh, note, 100);
				
				// 3rd (omit)
				//if (i == 1 || i == 4 || i== 5 || (Input::Key(KEY_INPUT_LALT) > 0 && (i == 2 || i == 3))) midiController.PlayHold(chordCh, note + 4, 100);
				//else midiController.PlayHold(chordCh, note + 3, 100);

				// 5th
				midiController.PlayHold(chordCh, note + 7, 100);

				// 7th
				if (Input::Key(KEY_INPUT_LCONTROL) > 0) {
					if ((Input::Key(KEY_INPUT_LALT) <= 0 && i == 1) || i == 4) midiController.PlayHold(chordCh, note + 11, 100);
					else midiController.PlayHold(chordCh, note + 10, 100);
				}
				// add9
				else if (Input::Key(KEY_INPUT_RIGHT) > 0) {
					midiController.PlayHold(chordCh, note + 14, 100);
				}
				else { // Octave
					midiController.PlayHold(chordCh, note + 12, 100);
				}
				

				// sus4 (12nd)
				if (Input::Key(KEY_INPUT_UP) > 0) midiController.PlayHold(chordCh, note + 17, 100);
				// 3rd
				else if (i == 1 || i == 4 || i== 5 || (Input::Key(KEY_INPUT_LALT) > 0 && (i == 2 || i == 3))) midiController.PlayHold(chordCh, note + 16, 100);
				else midiController.PlayHold(chordCh, note + 15, 100);
			}
		}

		if (Input::Key(KEY_INPUT_NUMPAD7) == 1) { // 転調♭
			baseNote += 11;
			baseNote %= 12;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD8) == 1) { // ナチュラル
			baseNote = 0;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD9) == 1) { // 転調♯
			baseNote += 1;
			baseNote %= 12;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480, 100);
		}
		
		if (Input::Key(KEY_INPUT_SPACE) == 1) {
			midiController.AllStop();
			conductor.PlayOrPause();
		}
	}


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

	
	return 0;
}

void MainScene::Draw(){
	// MIDIイベントを表示
	midiEventManager.draw();
	midiController.Draw();
	conductor.Draw(50, 50);
	gridRoll.Draw();
	commandInput.Draw(0, FMY - 20);

	// コードを表示
	static const int CHORD_X = 400, CHORD_Y = 440;
	static const int MARGIN = 50;
	if (Input::Key(KEY_INPUT_LCONTROL) > 0) {
		if (Input::Key(KEY_INPUT_LALT) > 0) DrawString(CHORD_X - MARGIN, CHORD_Y + MARGIN, CHORD_NAME[2][baseNote], WHITE);
		else DrawString(CHORD_X - MARGIN, CHORD_Y + MARGIN, CHORD_NAME[4][baseNote], WHITE);

		if (Input::Key(KEY_INPUT_LALT) > 0) DrawString(CHORD_X, CHORD_Y + MARGIN, CHORD_NAME[2][(baseNote + 2) % 12], WHITE);
		else DrawString(CHORD_X, CHORD_Y + MARGIN, CHORD_NAME[3][(baseNote + 2) % 12], WHITE);

		if (Input::Key(KEY_INPUT_LALT) > 0) DrawString(CHORD_X + MARGIN, CHORD_Y + MARGIN, CHORD_NAME[2][(baseNote + 4) % 12], WHITE);
		else DrawString(CHORD_X + MARGIN, CHORD_Y + MARGIN, CHORD_NAME[3][(baseNote + 4) % 12], WHITE);

		DrawString(CHORD_X - MARGIN, CHORD_Y, CHORD_NAME[4][(baseNote + 5) % 12], WHITE);
		DrawString(CHORD_X, CHORD_Y, CHORD_NAME[2][(baseNote + 7) % 12], WHITE);
		DrawString(CHORD_X + MARGIN, CHORD_Y, CHORD_NAME[3][(baseNote + 9) % 12], WHITE);
	}
	else {
		DrawString(CHORD_X - MARGIN, CHORD_Y + MARGIN, CHORD_NAME[0][baseNote], WHITE);
		DrawString(CHORD_X, CHORD_Y + MARGIN, CHORD_NAME[1][(baseNote + 2) % 12], WHITE);
		DrawString(CHORD_X + MARGIN, CHORD_Y + MARGIN, CHORD_NAME[1][(baseNote + 4) % 12], WHITE);
		DrawString(CHORD_X - MARGIN, CHORD_Y, CHORD_NAME[0][(baseNote + 5) % 12], WHITE);
		DrawString(CHORD_X, CHORD_Y, CHORD_NAME[0][(baseNote + 7) % 12], WHITE);
		DrawString(CHORD_X + MARGIN, CHORD_Y, CHORD_NAME[1][(baseNote + 9) % 12], WHITE);
	}
	// ベース、ルート音を表示
	DrawFormatString(CHORD_X - MARGIN, CHORD_Y - MARGIN, WHITE, "Base:%s(%d+%d)", CIRCLE_OF_FIFTH[0][baseNote] ,rootNote, baseNote);
	
	

	// TODO:テンポやプレイ状況、スケールを表示
	// コードを表示
	if (legacyMode)
		DrawStringToHandle(FMX/2 - 30, FMY/2 - 30, "Dm", WHITE, font);
}