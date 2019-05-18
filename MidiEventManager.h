#pragma once
#include "NoteOnEvent.h"
#include <vector>
#include <map>
#include "global.h"

class MidiEventManager {
	int div; // ����\
	int activeCh; // �A�N�e�B�u�ȃ`�����l��
	int seq; // �V�[�P���X�ʒu�i������ȁA�}�����Ɏg�p�j
	int mea; // ���݂̏��߈ʒu

	int BlockSize, BlockWidth, BlockHeight;
	int Base = 60;
	// �O���b�h�̈ʒu
	int x = 50;
	int y = 50;
	std::map<int, NoteOnEvent> noteMap[16];
public:
	MidiEventManager();
	void addNote(int Ch, int delta, int notenum, int gate, int vel=100);
	void deleteAllEvent();
	void autoCreate(int length=480);
	void Update(int focusch, int tick);
	void draw();

	int GetNoteData(int ch, int key) const;
	int GetGateData(int ch, int key) const;
	int GetVelData(int ch, int key) const;

	void keyUp();
	void keyDown();

	void LowerOctave();
	void HigherOctave();

	// �t�@�C���������݂̂��߂̃f�[�^���쐬���āA�n��
	int getMidiMsgForSMF(char* data);
};