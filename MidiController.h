#pragma once
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "global.h"
#include "PanelMenu.h"
#include "CurrentSound.h"
#include <vector>

class MidiController {
	// GM���Z�b�g�p�f�[�^
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
	void justPlay(int ch, int note, int gate, int vel);
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