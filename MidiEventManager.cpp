#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;

	// 1���Ԃ�̋x����ǉ�������
}

void MidiEventManager::addNote(int ch, int delta, int notenum, int gate, int vel) {

	// �m�[�g�I���ƃm�[�g�I�t(vel0)��ǉ�
	note.emplace_back(ch, delta, notenum, gate, vel);
	note.emplace_back(ch, gate, notenum, 0, 0);

	// map�̃L�[��128�i�m�[�g�i���o�[�j* ����\�i480�j* 4 * 128�����Ă�int�^�ɗ]�T�Ŏ��܂�
	INT_MAX;
	2147483647;
}

void MidiEventManager::autoCreate() {
	// �Ƃ肠�����A������Ȃ̔��t����������Ă݂�B
	// �����_���Ŕz�u
	static const int majorScale[8] = { 60, 62, 64, 65, 67, 69, 71, 72 };
	static const int chordC[4] = { 60, 64, 67, 72 };
	for (int i = 0; i < 10; i++)
		addNote(activeCh, 0, majorScale[GetRand(7)], div / 8);
}

void MidiEventManager::draw() {
	// MIDI�C�x���g��\���i���ݑ��쒆�̃`�����l����NoteON�C�x���g�̂݁j
	int j = 0;
	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		if (itr->GetVel() == 0) continue;
		DrawFormatString(0, 20 * j, WHITE, "CH:%d Delta:%d Note:%d Gate:%d", itr->GetCh(), itr->GetDelta(), itr->GetNote(), itr->GetGate());
		j++;
	}
}

int MidiEventManager::getMidiMsgForSMF(char* data) {
	int i = 0;
	// �ŏ��̏���

	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		// �f���^�^�C���̌v�Z
		int delta = itr->GetDelta();
		if (delta >> 21 != 0) data[i++] = (delta >> 21) | 128;
		if (delta >> 14 != 0) data[i++] = ((delta >> 14) & 127) | 128;
		if (delta >> 7 != 0) data[i++] = ((delta >> 7) & 127) | 128;
		data[i++] = delta & 127;

		data[i++] = (9 << 4) | itr->GetCh(); // ���o�C�g
		data[i++] = itr->GetNote(); // ���o�C�g
		data[i++] = itr->GetVel(); // ��O�o�C�g
	}

	// ���^�C�x���g�EEOF�i00 FF 2F 00�j
	data[i++] = 0x00; data[i++] = 0xFF; data[i++] = 0x2F; data[i++] = 0x00;
	return i;
}