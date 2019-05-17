#pragma once
#include "global.h"
#include "MidiEvent.h"
#include <vector>
#include <map>

class GridRoll {
	// �O���b�h�֌W
	int x, y;
	int BlockSize, BlockWidth, BlockHeight;
	
	// �m�[�g
	std::map<int, MidiEvent> midiEvent[16];
	
	// �V�[�P���X���
	int Base, Ch, Mea;

	void SetMidiEvent(int bit, int note, int val);
public:
	GridRoll();
	int GetNoteData(int ch, int bit) const;
	int GetGateData(int ch, int bit) const;

	void Update(int mea, int ch);
	void Draw() const;
	void Save(FILE* fp);
	void Load(FILE* fp);
	void DeleteOnePhrase();
	void DeleteAll();
	void Copy();
	void Paste();

	void KeyUp();
	void KeyDown();
};