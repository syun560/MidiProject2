#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;
}

void MidiEventManager::addNote(int ch, int tick, int notenum, int gate, int vel) {
	static int select = 480;

	// ノートオンとノートオフ(vel0)を追加
	note.emplace_back(ch, select, notenum, gate, vel);
	select += gate;
	note.emplace_back(ch, select, notenum, 0, 0);

	// ビットシフトして、ショートメッセージ（4バイト）を作成
	// エンディアンに注意
	// 書き込み、fwriteはどのタイミング？

	INT_MAX;
	2147483647;
}

void MidiEventManager::autoCreate() {
	// とりあえず、自動作曲の伴奏部分を作ってみる。
	addNote(activeCh, div, 60, div);
}

void MidiEventManager::draw() {
	// MIDIイベントを表示（現在操作中のチャンネルのNoteONイベントのみ）
	int j = 0;
	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		DrawFormatString(0, 20 * j, WHITE, "CH:%d Tick:%d Note:%d Gate:%d", itr->GetCh(), itr->GetTick(), itr->GetNote(), itr->GetGate());
		j++;
	}
}