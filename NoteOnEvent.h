#pragma once
#include "MidiEvent.h"

// TODO 継承をできればやめたい
class NoteOnEvent :public MidiEvent{
	int ch; // チャンネル
	int type; // タイプ（8=NoteOn、9=NoteOff)
	int tick; // 演奏開始位置からの時間
	int note, vel, gate;
	int delta; // 前のイベントからの経過時間
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