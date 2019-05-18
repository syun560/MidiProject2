#pragma once
#include "NoteOnEvent.h"
#include <vector>
#include <map>
#include "global.h"

class MidiEventManager {
	int div; // 分解能
	int activeCh; // アクティブなチャンネル
	int seq; // シーケンス位置（自動作曲、挿入時に使用）
	int mea; // 現在の小節位置

	int BlockSize, BlockWidth, BlockHeight;
	int Base = 60;
	// グリッドの位置
	int x = 50;
	int y = 50;
	std::map<int, NoteOnEvent> noteMap[16];
public:
	MidiEventManager();
	void addNote(int Ch, int delta, int notenum, int gate, int vel=100);
	void deleteAllEvent();
	void autoCreate(int length=480);
	void Update(int focusch, int tick);
	void draw();

	int GetNoteData(int ch, int key) const;
	int GetGateData(int ch, int key) const;
	int GetVelData(int ch, int key) const;

	void keyUp();
	void keyDown();

	void LowerOctave();
	void HigherOctave();

	// ファイル書き込みのためのデータを作成して、渡す
	int getMidiMsgForSMF(char* data);
};