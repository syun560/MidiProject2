#define _CRT_SECURE_NO_WARNINGS
#include "MainScene.h"
#include "Input.h"

// �ܓx��
static const char CIRCLE_OF_FIFTH[2][12][5] = {
	"C",  "G",  "D",  "A",    "E",    "B",   "G��",  "D��",  "A��", "E��", "B��", "F",
	"Am", "Em", "Bm", "F��m", "C��m", "G#m", "D��m", "B��m", "Fm",  "Cm",  "Gm",  "Dm"
};

static const char CHORD_NAME[6][12][8] = {
	"C", "D��", "D",  "E��", "E",  "F",  "G��", "G",  "A��", "A",  "B��", "B",
	"Cm", "C#m", "Dm", "D#m", "Em", "Fm", "F#m", "Gm", "G#m", "Am", "B��m", "Bm",
	"C7", "D��7", "D7",  "E��7", "E7",  "F7",  "G��7", "G7",  "A��7", "A7",  "B��7", "B7",
	"C7", "C#m7", "Dm7", "D#m7", "Em7", "Fm7", "F#m7", "Gm7", "G#m7", "Am7", "B��m7", "Bm7",
	"CM7", "D��M7", "DM7",  "E��M7", "EM7",  "FM7",  "G��M7", "GM7",  "A��M7", "AM7",  "B��M7", "BM7",
	"CM7", "C#mM7", "DmM7", "D#mM7", "EmM7", "FmM7", "F#mM7", "GmM7", "G#mM7", "AmM7", "B��mM7", "BmM7",
};

static const char COMMON_CHORD[2][7][8] = {
	"C", "Dm", "Em",  "F", "G", "Am",
	"CM7", "Dm7", "Em7",  "FM7", "G7", "Am7"
};

MainScene::MainScene() {
	font = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
	if(midiController.Init() == -1) printfDx("MIDI���������s\n");
	InputMode = false;
	legacyMode = false; // �̂̋@�\
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
	// ����n
	static const int KENBAN_MAX = 18;
	int kenban[KENBAN_MAX];
	kenban[0] = Input::Key(KEY_INPUT_Z); // ���[�g��
	kenban[1] = Input::Key(KEY_INPUT_S);
	kenban[2] = Input::Key(KEY_INPUT_X);
	kenban[3] = Input::Key(KEY_INPUT_D);
	kenban[4] = Input::Key(KEY_INPUT_C);
	kenban[5] = Input::Key(KEY_INPUT_V);
	kenban[6] = Input::Key(KEY_INPUT_G);
	kenban[7] = Input::Key(KEY_INPUT_B);
	kenban[8] = Input::Key(KEY_INPUT_H);
	kenban[9] = Input::Key(KEY_INPUT_N);
	kenban[10] = Input::Key(KEY_INPUT_J);
	kenban[11] = Input::Key(KEY_INPUT_M);
	kenban[12] = Input::Key(KEY_INPUT_COMMA); // ���[�g���i�I�N�^�[�u��j
	kenban[13] = Input::Key(KEY_INPUT_L);
	kenban[14] = Input::Key(KEY_INPUT_PERIOD);
	kenban[15] = Input::Key(KEY_INPUT_SEMICOLON);
	kenban[16] = Input::Key(KEY_INPUT_SLASH);
	kenban[17] = Input::Key(KEY_INPUT_BACKSLASH);


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
				unsigned char data[10000]; // TODO �ł���Γ��I�Ɍ��肷�ׂ�
				int size = midiEventManager.getMidiMsgForSMF(data);
				if (smfio.write("output.mid", data, size) == -1) printfDx("�������ݎ��s\n");
				else printfDx("�������ݐ���\n");
			}
			else if (Input::Key(KEY_INPUT_O) == 1) { // �t�@�C���ǂݍ���
				// FileSave("data.dat");
				if (smfio.read("kaeru.mid") == -1) printfDx("�ǂݍ��ݎ��s\n");
				else {
					for (int i = 0; i < smfio.getTrackNum(); ++i) {
						double bpm = midiEventManager.loadMidiMsgFromSMF(i, smfio.getTrackData(i), smfio.getTrackSize(i));
						if (bpm != 0.0) conductor.SetTempo(bpm);
					}
					printfDx("�ǂݍ��ݐ���\n");
					smfio.draw();
				}
			}
			else if (Input::Key(KEY_INPUT_A) == 1) { // �Đ�
				if (PlayMusic("output.mid", DX_PLAYTYPE_BACK) == -1) printfDx("�G���[����\n");
				else printfDx("%s���Đ�\n", "output.mid");
			}
			else if (Input::Key(KEY_INPUT_RETURN) == 1) { // �������
				midiEventManager.deleteAllEvent();
				midiEventManager.autoCreate(480 * 3 * 32);
			}
			else if (Input::Key(KEY_INPUT_V) == 1) gridRoll.Paste();
			//else if (Input::Key(KEY_INPUT_X) == 1) StopMusic(); // MIDI��~
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

		if (Input::Key(KEY_INPUT_NUMPAD7) == 1) { // �]����
			baseNote += 5;
			baseNote %= 12;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD8) == 1) { // �i�`������
			baseNote = 0;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD9) == 1) { // �]����
			baseNote += 7;
			baseNote %= 12;
			midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480, 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD4) > 0) { // �W�̘a��
			if (Input::Key(KEY_INPUT_LCONTROL) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(5), 480 * 2, 100, false, true);
			else midiController.PlayChordHold(midiController.GetFocusCh(), tenkai(5), 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD5) > 0) { // �T�̘a��
			if (Input::Key(KEY_INPUT_LCONTROL) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(0), 480 * 2, 100, false, true);
			else midiController.PlayChordHold(midiController.GetFocusCh(), tenkai(0), 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD6) > 0) { // �X�̘a��
			if (Input::Key(KEY_INPUT_LCONTROL) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(7), 480 * 2, 100, false, true);
			else midiController.PlayChordHold(midiController.GetFocusCh(), tenkai(7), 100);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD1) > 0) { // �Um�̘a��
			if (Input::Key(KEY_INPUT_LCONTROL) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(2), 480 * 2, 100, true, true);
			else midiController.PlayChordHold(midiController.GetFocusCh(), tenkai(2), 100, true);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD2) > 0) { // �Ym�̘a��
			if (Input::Key(KEY_INPUT_LCONTROL) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(9), 480 * 2, 100, true, true);
			else midiController.PlayChordHold(midiController.GetFocusCh(), tenkai(9), 100, true);
		}
		else if (Input::Key(KEY_INPUT_NUMPAD3) > 0) { // �Vm�̘a��
			if (Input::Key(KEY_INPUT_LCONTROL) > 0) midiController.PlayChord(midiController.GetFocusCh(), tenkai(4), 480 * 2, 100, true, true);
			else midiController.PlayChordHold(midiController.GetFocusCh(), tenkai(4), 100, true);
		}
		if (Input::Key(KEY_INPUT_SPACE) == 1) {
			midiController.AllStop();
			conductor.PlayOrPause();
		}
	}


	// �X�V
	// ������tick�̒l�𓾂�
	int c = conductor.Update();
	midiController.Update(conductor.GetDelta());
	// TODO �����悤�ȏ������d�����Ă�
	gridRoll.Update(conductor.GetMea(), midiController.GetFocusCh());
	midiEventManager.Update(midiController.GetFocusCh(), conductor.GetMea());

	// �V�[�P���X
	if (c != -1) { // PLAYING��������
		for (int ch = 0; ch < 16; ch++) {
			key1 = (int)conductor.GetPreTick();
			key2 = (int)c;
			for (int key = key1; key < key2; key++) {
				// TODO ���̂R�̊֐����\���̂Ȃǂ��g�p���ĂЂƂɂ܂Ƃ߂����Amap��lower�֐����g������
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
	// MIDI�C�x���g��\��
	midiEventManager.draw();
	midiController.Draw();
	conductor.Draw(50, 50);
	gridRoll.Draw();
	commandInput.Draw(0, FMY - 20);

	// ������@��\��
	/*if (Input::Key(KEY_INPUT_M) > 0) {
		static const int INFO_X = 400;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(INFO_X - 20, 80, FMX - 20, 240, BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawFormatString(INFO_X, 80, WHITE, "������@\nO�L�[�œǍ�\nS�L�[�ŕۑ�\nA�L�[�ōĐ�\nD�L�[�Œ�~\nC�L�[�ŃN���A\nESC�ŏI��");
	}*/

	// �R�[�h��\��
	static const int CHORD_X = 400, CHORD_Y = 400;
	static const int MARGIN = 50;
	int chordType = 0;
	if (Input::Key(KEY_INPUT_LCONTROL) > 0) chordType = 2;
	
	DrawString(CHORD_X - MARGIN, CHORD_Y         , CHORD_NAME[0 + chordType][(baseNote + 5) % 12], WHITE);
	DrawString(CHORD_X         , CHORD_Y         , CHORD_NAME[0 + chordType][(baseNote + 0) % 12], WHITE);
	DrawString(CHORD_X + MARGIN, CHORD_Y         , CHORD_NAME[0 + chordType][(baseNote + 7) % 12], WHITE);
	DrawString(CHORD_X - MARGIN, CHORD_Y + MARGIN, CHORD_NAME[1 + chordType][(baseNote + 2) % 12], WHITE);
	DrawString(CHORD_X         , CHORD_Y + MARGIN, CHORD_NAME[1 + chordType][(baseNote + 9) % 12], WHITE);
	DrawString(CHORD_X + MARGIN, CHORD_Y + MARGIN, CHORD_NAME[1 + chordType][(baseNote + 4) % 12], WHITE);

	// TODO:�e���|��v���C�󋵁A�X�P�[����\��
	// �R�[�h��\��
	if (legacyMode)
		DrawStringToHandle(FMX/2 - 30, FMY/2 - 30, "Dm", WHITE, font);
}