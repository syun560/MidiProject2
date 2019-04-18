#pragma once
#include "NoteOnEvent.h"
#include <vector>
#include "global.h"

class MidiEventManager {
	int div; // ����\
	int activeCh; // �A�N�e�B�u�ȃ`�����l��
	int seq; // �V�[�P���X�ʒu
	std::vector<NoteOnEvent> note[16]; // �ł����Map�ŊǗ�
public:
	MidiEventManager();
	void addNote(int Ch, int delta, int notenum, int gate, int vel=100);
	void deleteAllEvent();
	void autoCreate(int length=480);
	void draw();

	// �t�@�C���������݂̂��߂̃f�[�^���쐬���āA�n��
	int getMidiMsgForSMF(char* data);
};