#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;
}

void MidiEventManager::addNote(int ch, int tick, int notenum, int gate, int vel) {
	static int select = 480;

	// �m�[�g�I���ƃm�[�g�I�t(vel0)��ǉ�
	note.emplace_back(ch, select, notenum, gate, vel);
	select += gate;
	note.emplace_back(ch, select, notenum, 0, 0);

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
		DrawFormatString(0, 20 * j, WHITE, "CH:%d Tick:%d Note:%d Gate:%d", itr->GetCh(), itr->GetTick(), itr->GetNote(), itr->GetGate());
		j++;
	}
}