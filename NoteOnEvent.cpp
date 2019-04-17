#include "NoteOnEvent.h"

NoteOnEvent::NoteOnEvent() {
	ch = 0;
	type = 8; // NoteOnイベント
	gate = 480;
	tick = 480 * 4;
	note = 60;
	vel = 100;
}

NoteOnEvent::NoteOnEvent(int Ch, int Tick, int Note, int Gate, int Vel) {
	ch = Ch;
	type = 8; // NoteOnイベント
	gate = Gate;
	tick = Tick;
	vel = Vel;
	note = Note;
}

int NoteOnEvent::GetCh() const { return ch; }
int NoteOnEvent::GetGate() const { return gate; }
int NoteOnEvent::GetNote() const { return note; }
int NoteOnEvent::GetTick() const { return tick; }