#pragma once
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "global.h"
#include "PanelMenu.h"
#include "CurrentSound.h"
#include <vector>

class MidiController {
	// GMリセット用データ
	BYTE abyGMReset[6] = { 0xf0, 0x7e, 0x7f, 0x09, 0x01, 0xf7 };
	MIDIHDR mhMidi;
	HMIDIOUT g_hMidi;
	const int CH_MAX;
	int NowProgram[16];
	PanelMenu panel;
	std::vector<CurrentSound> cs;
	void Stop(int ch, int note);
public:
	MidiController();
	~MidiController();
	int Init();
	void Update(double delta);
	void Play(int ch, int note, int gate, int vel);
	void PlayHold(int ch, int note, int vel);
	void PlayChord(int ch, int baseNote, int gate, int vel, bool isMinor = false, bool isSeventh = false);
	void PlayChordHold(int ch, int baseNote, int vel, bool isMinor=false, bool isSeventh=false);
	void AllStop();
	void ProgramChange(int ch, int num);
	void NextProgram();
	void PreProgram();
	void NextCh();
	void PreCh();

	int GetFocusCh() const;
	void Save(FILE* fp);
	void Load(FILE* fp);
	void Draw() const;
};