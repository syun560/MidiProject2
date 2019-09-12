#include "NoteOnEvent.h"

NoteOnEvent::NoteOnEvent() {
	ch = 0;
	type = 8; // NoteOnイベント
	gate = 480;
	tick = 480 * 4;
	note = 60;
	vel = 100;
	delta = 0;
}

NoteOnEvent::NoteOnEvent(int Ch, int Delta, int Note, int Gate, int Vel) {
	ch = Ch;
	type = 8; // NoteOnイベント
	gate = Gate;
	//tick = Tick;
	delta = Delta;
	vel = Vel;
	note = Note;
}

void NoteOnEvent::SetGate(int val) {
	gate = val;
}

void NoteOnEvent::transpose(int val) {
	note += val;
	if (note >= 128 || note < 0) note -= val;
}

int NoteOnEvent::getMidiShortMsg() const{
	return vel << 16 | note << 8 | 9 << 4 | ch;
}

int NoteOnEvent::GetCh() const { return ch; }
int NoteOnEvent::GetGate() const { return gate; }
int NoteOnEvent::GetNote() const { return note; }
int NoteOnEvent::GetDelta() const { return delta; }
int NoteOnEvent::GetVel() const { return vel; }