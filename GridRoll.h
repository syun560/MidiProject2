#pragma once
#include "global.h"
#include "MidiEvent.h"
#include <vector>
#include <map>

class GridRoll {
	// グリッド関係
	int x, y;
	int BlockSize, BlockWidth, BlockHeight;
	
	// ノート
	std::map<int, MidiEvent> midiEvent[16];
	
	// シーケンス情報
	int Base, Ch, Mea;

	void SetMidiEvent(int bit, int note, int val);
public:
	GridRoll();
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