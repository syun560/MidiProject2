#pragma once
#include "MidiEvent.h"

class NoteOnEvent :public MidiEvent{
	int ch; // �`�����l��
	int type; // �^�C�v�i8=NoteOn�A9=NoteOff)
	int tick; // ���t�J�n�ʒu����̎���
	int note, vel, gate;
public:
	NoteOnEvent();
	NoteOnEvent(int Ch, int tick, int Note, int Vel, int Gate);
	int GetCh() const;
	int GetNote() const;
	int GetGate() const;
	int GetTick() const;
};