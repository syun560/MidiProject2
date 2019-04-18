#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 1;
}

void MidiEventManager::addNote(int ch, int delta, int notenum, int gate, int vel) {
	// 1���Ԃ�̋x����ǉ�������
	if (seq == 0) {
		note[ch].emplace_back(ch, div * 4, 60, 0, 0);
		seq += div * 4;
		return;
	}

	// �m�[�g�I���ƃm�[�g�I�t(vel0)��ǉ�
	note[ch].emplace_back(ch, delta, notenum, gate, vel);
	note[ch].emplace_back(ch, gate, notenum, 0, 0);

	// map�̃L�[��128�i�m�[�g�i���o�[�j* ����\�i480�j* 4 * 128�����Ă�int�^�ɗ]�T�Ŏ��܂�
	INT_MAX;
	2147483647;

	// ���݂̃V�[�P���X�ʒu���ړ�
	seq += delta + gate;
}

void MidiEventManager::autoCreate(int length) {
	// �Ƃ肠�����A������Ȃ̔��t����������Ă݂�B
	// �����_���Ŕz�u�i���܂�ɔ�є�т̉��t�͂�߂�j
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
	note[activeCh].clear();
	seq = 0;
}

void MidiEventManager::draw() {
	// MIDI�C�x���g��\���i���ݑ��쒆�̃`�����l����NoteON�C�x���g�̂݁j
	int j = 0;
	for (auto itr = note[activeCh].cbegin(); itr != note[activeCh].cend(); itr++) {
		if (itr->GetVel() == 0) continue;
		DrawFormatString(100, 20 * j, WHITE, "CH:%d Delta:%d Note:%d Gate:%d", itr->GetCh(), itr->GetDelta(), itr->GetNote(), itr->GetGate());
		j++;
	}
}

int MidiEventManager::getMidiMsgForSMF(char* data) {
	int i = 0;
	// �ŏ��̏���

	for (auto itr = note[activeCh].cbegin(); itr != note[activeCh].cend(); itr++) {
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