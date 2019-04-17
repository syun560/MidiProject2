#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;
}

void MidiEventManager::addNote(int ch, int tick, int notenum, int gate, int vel) {

	// �m�[�g�I���ƃm�[�g�I�t(vel0)��ǉ�
	note.emplace_back(ch, 0, notenum, gate, vel);
	note.emplace_back(ch, div, notenum, 0, 0);

	// �r�b�g�V�t�g���āA�V���[�g���b�Z�[�W�i4�o�C�g�j���쐬
	// �G���f�B�A���ɒ���
	// �������݁Afwrite�͂ǂ̃^�C�~���O�H

	INT_MAX;
	2147483647;
}

void MidiEventManager::autoCreate() {
	// �Ƃ肠�����A������Ȃ̔��t����������Ă݂�B
	addNote(activeCh, div, 60, div);
}

void MidiEventManager::draw() {
	// MIDI�C�x���g��\���i���ݑ��쒆�̃`�����l����NoteON�C�x���g�̂݁j
	int j = 0;
	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		DrawFormatString(0, 20 * j, WHITE, "CH:%d Delta:%d Note:%d Gate:%d", itr->GetCh(), itr->GetDelta(), itr->GetNote(), itr->GetGate());
		j++;
	}
}

int MidiEventManager::getMidiMsgForSMF(char* data) {
	int i = 0;
	// �ŏ��̏���

	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		// �f���^�^�C�����v�Z���āA�i�[����B
		int delta = itr->GetDelta();
		if (delta >> 21 != 0) {
			data[i] = (delta >> 21) | 128;
			i++;
		}
		if (delta >> 14 != 0) {
			data[i] = ((delta >> 14) & 127) | 128;
			i++;
		}
		if (delta >> 7 != 0) {
			data[i] = ((delta >> 7) & 127) | 128;
			i++;
		}
		data[i] = delta & 127;
		i++;
		// ���o�C�g
		data[i] = (9 << 4) | itr->GetCh();
		i++;
		// ���o�C�g
		data[i] = itr->GetNote();
		i++;
		// ��O�o�C�g
		data[i] = itr->GetVel();
		i++;
	}
	return i;
}

int MidiEventManager::getMidiEventNum() {
	return note.size();
}