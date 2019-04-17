#pragma once
#include "NoteOnEvent.h"
#include <vector>
#include "global.h"

class MidiEventManager {
	int div; // 分解能
	int activeCh; // アクティブなチャンネル
	std::vector<NoteOnEvent> note; // できればMapで管理
public:
	MidiEventManager();
	void addNote(int Ch, int tick, int notenum, int gate, int vel=100);
	void autoCreate();
	void draw();

	// ファイル書き込みのためのデータを作成して、渡す
	int getMidiMsgForSMF(char* data);

	int getMidiEventNum();
};