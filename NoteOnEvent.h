#pragma once
#include "MidiEvent.h"

// TODO �p�����ł���΂�߂���
class NoteOnEvent :public MidiEvent{
	int ch; // �`�����l��
	int type; // �^�C�v�i8=NoteOn�A9=NoteOff)
	int tick; // ���t�J�n�ʒu����̎���
	int note, vel, gate;
	int delta; // �O�̃C�x���g����̌o�ߎ���
public:
	NoteOnEvent();
	NoteOnEvent(int Ch, int delta, int Note, int Vel, int Gate);
	int getMidiShortMsg() const;

	int GetCh() const;
	int GetNote() const;
	int GetGate() const;
	int GetDelta() const;
	int GetVel() const;
};