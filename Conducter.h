#pragma once
#include "global.h"

class Conducter {
	int mea;
	double bpm, tick, pretick, delta, Resolution, bit;
	int now;
	int Beat;
	bool Repeat;
	char* Key;
	enum State {
		PLAYING, PAUSE,
	};
	State state;
public:
	Conducter();
	void Faster();
	void Slower();
	void NextMea();
	void PreMea();
	void FirstBeat();
	void EndBeat();
	int Update();
	void Stop();

	void SetRepeat();
	void Pause();
	void Save(FILE* fp);
	void Load(FILE* fp);
	void Draw(int x, int y) const;
	
	double GetDelta() const;
	double GetPreTick() const;
	int GetMea() const;
};