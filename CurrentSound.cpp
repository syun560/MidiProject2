#include "CurrentSound.h"

CurrentSound::CurrentSound() {
	Gate = GateRemain = 0.0;
	Note = Ch = 0;
}

CurrentSound::CurrentSound(int ch, int note, double gate, bool legato) {
	Gate = GateRemain = gate;
	Note = note;
	Ch = ch;
	Legato = legato;
}

int CurrentSound::Update(double delta) {
	GateRemain -= delta;
	if (GateRemain <= 0.0) {
		return -1;
	}
	return 0;
}

bool CurrentSound::IsCollide(int ch, int note) const{
	if (Ch == ch && Note == note) return true;
	else return false;
}

int CurrentSound::GetCh() const{
	return Ch;
}

int CurrentSound::GetNote() const{
	return Note;
}

double CurrentSound::GetGate() const{
	return GateRemain;
}