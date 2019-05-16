#pragma once
#include "global.h"

class MidiEvent {
	int id;
	int Ch, Mea, Bit, Tick, Step, Event, Gate, Value;
public:
	MidiEvent();
	MidiEvent(int ch, int mea, int bit, int ev, int gate, int value);
	void Draw(int x, int y) const;
	
	int GetNote() const;
	int GetGate() const;
	int GetBit() const;
};