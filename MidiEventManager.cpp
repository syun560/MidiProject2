#include "MidiEventManager.h"
#include "math.h"

static const char keyName[12][4] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
static const int ScaleC[13] = { 1,1,0,1,0,1,0,1,1,0,1,0,1 };
static const int avoidcol = GetColor(80, 80, 80);
static const int gatecol = GetColor(200, 200, 255);
static const int basecol = GetColor(80, 180, 202);

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;
	BlockSize = 16;
	BlockWidth = 16;
	BlockHeight = 24;
	Base = 60;
	// �O���b�h�̈ʒu
	x = 50;
	y = 50;
}

void MidiEventManager::addNote(int ch, int delta, int notenum, int gate, int vel) {
	// TODO �����Ŗ{���͊y��̃Z�b�g�A�b�v���s������
	// 1���Ԃ�̋x����ǉ�������
	if (seq == 0) {
		//noteMap[ch].emplace(seq, NoteOnEvent(ch, div * 4, 60, 0, 0));
		seq += div * 4;
		return;
	}

	// key = resolution(480) * note(128) * beat(4)* mea(128) = 31457280 < INT_MAX(2147483647)
	int mapKey1 = (seq + delta) * 128 + notenum;
	int mapKey2 = (seq + delta + gate - 1) * 128 + notenum; // NoteOff�̃L�[�͎���NoteOn�Əd�Ȃ�Ȃ��悤��-1����
	noteMap[ch].emplace(mapKey1, NoteOnEvent(ch, delta, notenum, gate, vel));
	noteMap[ch].emplace(mapKey2, NoteOnEvent(ch, gate, notenum, 0, 0));

	// ���݂̃V�[�P���X�ʒu���ړ�
	seq += delta + gate;
}

void MidiEventManager::addStartNote(int delta, int ch, int notenum, int vel) {
	int key = seq + delta;
	noteMap[ch].emplace(key, NoteOnEvent(ch, delta, notenum, 400, vel));
	seq += delta;
}

void MidiEventManager::addEndNote(int delta, int ch, int notenum) {
	int key = seq + delta - 1;
	noteMap[ch].emplace(key, NoteOnEvent(ch, delta, notenum, 0, 0));
	seq += delta;
}

void MidiEventManager::autoCreate(int length) {
	seq = 0;
	// �y���^�g�j�b�N�E�X�P�[���Ń����_���ɔz�u�i���܂�ɔ�є�т̉��t�͂�߂�j
	static const int majorScale[8] = { 60, 62, 64, 65, 67, 69, 71, 72 };
	static const int chordC[4] = { 60, 64, 67, 72 };
	static const int kokken[] = { 54, 56, 58, 61, 63, 66, 68, 70, 73, 75, 78 };
	static const int gate[8] = { div / 2 , div / 2, div / 2, div / 2, div, div, div, div * 3 / 2};
	static const int rest[] = { 0, 0, 0, 0, 0, div / 2};
	int key, preKey;
	key = preKey = GetRand(10);
	while (seq < length) {
		preKey = key; // �O�ɉ������������o���Ă���
		do {
			key = preKey - 2 + GetRand(4);
		} while (key < 0 || key > 10);
		addNote(activeCh, rest[GetRand(5)], kokken[key], gate[GetRand(7)], 90 + GetRand(20));
	}	
}

void MidiEventManager::deleteAllEvent() {
	noteMap[activeCh].clear();
	seq = 0;
}

int MidiEventManager::GetNoteData(int ch, int key) const {
	auto itr = noteMap[ch].find(key);
	if (itr != noteMap[ch].end()) {
		return itr->second.GetNote();
	}
	else return -1;
}
int MidiEventManager::GetGateData(int ch, int key) const {
	auto itr = noteMap[ch].find(key);
	if (itr != noteMap[ch].end()) {
		return itr->second.GetGate();
	}
	else return -1;
}
int MidiEventManager::GetVelData(int ch, int key) const {
	auto itr = noteMap[ch].find(key);
	if (itr != noteMap[ch].end()) {
		return itr->second.GetVel();
	}
	else return -1;
}

void MidiEventManager::Update(int focusch, int dmea) {
	activeCh = focusch;
	mea = dmea;
}

void MidiEventManager::keyDown() {
	for (auto itr = noteMap[activeCh].begin(); itr != noteMap[activeCh].end(); itr++) {
		itr->second.transpose(-12);
	}
}

void MidiEventManager::keyUp() {
	for (auto itr = noteMap[activeCh].begin(); itr != noteMap[activeCh].end(); itr++) {
		itr->second.transpose(12);
	}
}

void MidiEventManager::draw() {
	// Base����\��
	DrawFormatString(x - 30, y + BlockHeight * BlockSize, WHITE, "C%d", Base / 12);
	DrawFormatString(x - 30, y + BlockHeight * BlockSize / 2, WHITE, "C%d", Base / 12 + 1);
	DrawFormatString(x - 30, y, WHITE, "C%d", Base / 12 + 2);

	// �O���b�h��`��
	for (int i = 0; i < BlockWidth; i++) {
		for (int j = 0; j <= BlockHeight; j++) {
			if (j % 12 == 0) DrawEdgeBox(i * BlockSize + x, j * BlockSize + y, (i + 1)*BlockSize + x, (j + 1)*BlockSize + y, basecol, avoidcol);
			else if (ScaleC[j % 12] == 0) DrawEdgeBox(i * BlockSize + x, j * BlockSize + y, (i + 1)*BlockSize + x, (j + 1)*BlockSize + y, BLACK, avoidcol);
			else DrawEdgeBox(i * BlockSize + x, j * BlockSize + y, (i + 1)*BlockSize + x, (j + 1)*BlockSize + y, WHITE, avoidcol
			);
		}
	}

	// MIDI�C�x���g��\���i���ݑ��쒆�̃`�����l����NoteON�C�x���g�̂݁j
	int j = 0;
	// Condoctor�̓����ɍ��킹�ĕ\������悤�ɂ������B
	int firsttick = mea * div * 4;
	int endtick = (mea + 1) * div * 4 - 1;
	DrawFormatString(540, 50, WHITE, " Tick Event  Gate Vel");
	for (auto itr = noteMap[activeCh].lower_bound(firsttick); itr != noteMap[activeCh].upper_bound(endtick); itr++) {
		if (itr->second.GetVel() == 0) continue;
		int tick = itr->first % (div * 4);
		int delta = itr->second.GetDelta();
		int note = itr->second.GetNote();
		int gate = itr->second.GetGate();
		int vel = itr->second.GetVel();
		DrawFormatString(540, 70 + 20 * j, WHITE, "%5d %2s%d(%d) %d %d", tick, keyName[note % 12], note / 12, note, gate, vel);
		if (++j == 20) break;

		// Note��`��
		int n = note - Base; // Base���牽��̉����H
		if (0 <= n && n < BlockHeight) {
			int x1 = BlockSize * tick / 120.0 + x;
			int y1 = (BlockHeight - n) * BlockSize + y;
			int x2 = x1 + BlockSize * gate / 120.0;
			int y2 = y1 + BlockSize;
			DrawEdgeBox(x1, y1, x2, y2, GREEN);
		}


	}
	DrawBox(540, 50, 740, 50 + 20 * 20, GREEN, FALSE);
	// DrawBox(540, 50, 740, 50 + 20, YELLOW, FALSE);
}

void MidiEventManager::LowerOctave() {
	Base -= 12;
	if (Base < 0) Base += 12;
}

void MidiEventManager::HigherOctave() {
	Base += 12;
	if (Base > 127) Base -= 12;
}

double MidiEventManager::loadMidiMsgFromSMF(int track, unsigned char* data, int size) {
	int i = 0;
	double bpm = 0.0f;
	int a, b, c, d;
	printfDx("�V�[�P���X�J�n\n");
	while (i < size) {
		//printfDx("%02X ", data[i++]);
		//if (i%16 == 0) printfDx("\n");

		// �f���^�^�C��
		int delta = 0;
		while ((d = data[i++]) & 0x80) {
			delta = delta | (d & 0x7F);
			delta <<= 7;
		}
		delta = delta | d;
		printfDx("(%d) ", delta);

		// �`�����l�����b�Z�[�W
		if ((data[i] & 0xF0) == 0x90) {
			// �m�[�g�I��
			int ch = data[i++] & 0x0F;
			int note = data[i++];
			int vel = data[i++];
			printfDx("track:%d, Ch:%d, Note:%d, Vel:%d", track, ch, note, vel);
			addStartNote(delta, track, note, vel);

			continue;
		}
		if ((data[i] & 0xF0) == 0x80) {
			// �m�[�g�I�t
			int ch = data[i++] & 0x0F;
			int note = data[i++];
			int vel = data[i++];
			printfDx("track:%d, Ch:%d, Note:%d, Vel:%d \n", track, ch, note, vel);
			addEndNote(delta, track, note);
			continue;
		}

		// ���^�C�x���g
		switch (data[i++]) {
		case 0xFF: // ���^�C�x���g
			switch (data[i++]) {
			case 0x02: // ���쌠
				i += data[i++]; // ��΂�
				printfDx("���쌠\n");
				break;
			case 0x03: // �V�[�P���X��
				i += data[i++]; // ��΂�
				printfDx("�V�[�P���X\n");
				break;
			case 0x51: // �e���|
				i++; // �f�[�^���Ȃ̂Ŕ�΂�
				a = (int)data[i++] << 16;
				b = (int)data[i++] << 8;
				c = (int)data[i++];
				bpm = 60.0 * 1000000 / (a | b | c);
				// TODO �Ȃ����ꂪ���҂������ʂɂȂ�Ȃ��̂��ǂ��킩��Ȃ�
				//bpm = 60.0 * 1000000 / (((int)data[i++] << 16) | ((int)data[i++] << 8) | ((int)data[i++]));
				printfDx("bpm = %.2f\n", bpm);
				break;
			case 0x58: // ���q�ݒ�[4byte]
				i++; // �f�[�^���Ȃ̂Ŕ�΂�
				printfDx("���q�F%d/%d\n", (int)pow((double)data[i++], 2.0), data[i++]);
				i += 2; // �d�v�łȂ����
				break;
			case 0x2F: // �g���b�N�I�[
				switch (data[i++]) {
				case 0x00: // �g���b�N�I�[
					printfDx("End Of Track (i = %d, size = %d)\n", i, size);
					break;
				}
				break;
			}
			break;
		}
	}

	// �ォ��Gate��ݒ肷��
	for (auto itr = noteMap[track].begin(); itr != noteMap[track].end(); ++itr) {
		if (itr->second.GetVel() > 0) {
			for (auto itr2 = noteMap[track].upper_bound(itr->first); itr2 != noteMap[track].end(); ++itr2) {
				if (itr2->second.GetVel() == 0) {
					itr->second.SetGate(itr2->first - itr->first);
					break;
				}
			}
		}
	}
	return bpm;
}

int MidiEventManager::getMidiMsgForSMF(unsigned char* data) {
	int i = 0;
	// �ŏ��̏���
	for (auto itr = noteMap[activeCh].cbegin(); itr != noteMap[activeCh].cend(); itr++) {
		// �f���^�^�C���̌v�Z
		int delta = itr->second.GetDelta();
		if (delta >> 21 != 0) data[++i] = (delta >> 21) | 128;
		if (delta >> 14 != 0) data[i++] = ((delta >> 14) & 127) | 128;
		if (delta >> 7 != 0) data[i++] = ((delta >> 7) & 127) | 128;
		data[i++] = delta & 127;

		data[i++] = (9 << 4) | itr->second.GetCh(); // ���o�C�g
		data[i++] = itr->second.GetNote(); // ���o�C�g
		data[i++] = itr->second.GetVel(); // ��O�o�C�g
	}

	// ���^�C�x���g�EEOF�i00 FF 2F 00�j
	data[i++] = 0x00; data[i++] = 0xFF; data[i++] = 0x2F; data[i++] = 0x00;
	return i;
}