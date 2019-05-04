#pragma once
#include "global.h"

class CurrentSound {
	double Gate, GateRemain;
	int Note, Ch;
	bool Legato;
public:
	CurrentSound();
	CurrentSound(int ch, int note, double gate, bool legato=false);
	int Update(double delta);
	int GetNote() const;
	int GetCh() const;
	double GetGate() const;

	bool IsCollide(int ch, int note) const;
};