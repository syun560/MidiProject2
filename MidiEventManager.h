#pragma once
#include "NoteOnEvent.h"
#include <vector>
#include "global.h"

class MidiEventManager {
	int div; // ����\
	int activeCh; // �A�N�e�B�u�ȃ`�����l��
	std::vector<NoteOnEvent> note; // �ł����Map�ŊǗ�
public:
	MidiEventManager();
	void addNote(int Ch, int tick, int notenum, int gate, int vel=100);
	void autoCreate();
	void draw();

	// �t�@�C���������݂̂��߂̃f�[�^���쐬���āA�n��
	int getMidiMsgForSMF(char* data);

	int getMidiEventNum();
};