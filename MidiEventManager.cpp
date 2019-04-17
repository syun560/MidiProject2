#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;
}

void MidiEventManager::addNote(int ch, int tick, int notenum, int gate, int vel) {

	// ノートオンとノートオフ(vel0)を追加
	note.emplace_back(ch, 0, notenum, gate, vel);
	note.emplace_back(ch, div, notenum, 0, 0);

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
		DrawFormatString(0, 20 * j, WHITE, "CH:%d Delta:%d Note:%d Gate:%d", itr->GetCh(), itr->GetDelta(), itr->GetNote(), itr->GetGate());
		j++;
	}
}

int MidiEventManager::getMidiMsgForSMF(char* data) {
	int i = 0;
	// 最初の処理

	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		// デルタタイムを計算して、格納する。
		int delta = itr->GetDelta();
		if (delta >> 21 != 0) {
			data[i] = (delta >> 21) | 128;
			i++;
		}
		if (delta >> 14 != 0) {
			data[i] = ((delta >> 14) & 127) | 128;
			i++;
		}
		if (delta >> 7 != 0) {
			data[i] = ((delta >> 7) & 127) | 128;
			i++;
		}
		data[i] = delta & 127;
		i++;
		// 第一バイト
		data[i] = (9 << 4) | itr->GetCh();
		i++;
		// 第二バイト
		data[i] = itr->GetNote();
		i++;
		// 第三バイト
		data[i] = itr->GetVel();
		i++;
	}
	return i;
}

int MidiEventManager::getMidiEventNum() {
	return note.size();
}