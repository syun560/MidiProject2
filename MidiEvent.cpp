#include "MidiEvent.h"

MidiEvent::MidiEvent() {
	id = 0;
	Ch = Mea = Bit = Tick = Step = Event = Gate = Value = 0;
}

MidiEvent::MidiEvent(int ch, int mea, int bit, int ev, int gate, int value) {
	Ch = ch;
	Mea = mea;
	Bit = bit;
	Tick = Bit * 120;
	Step = 0;
	Event = ev;
	Gate = gate;
	Value = value;
}

void MidiEvent::Draw(int x, int y) const {
	DrawFormatString(x, y, WHITE,"%d/%d/%d Note[%d]/%d/%d", Ch, Mea, Tick, Event, Gate, Value);
}

int MidiEvent::GetNote() const {
	return Event;
}
int MidiEvent::GetGate() const {
	return Gate;
}
int MidiEvent::GetBit() const {
	return Bit;
}