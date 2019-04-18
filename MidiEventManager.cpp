#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;

	// 1拍ぶんの休符を追加したい
}

void MidiEventManager::addNote(int ch, int delta, int notenum, int gate, int vel) {

	// ノートオンとノートオフ(vel0)を追加
	note.emplace_back(ch, delta, notenum, gate, vel);
	note.emplace_back(ch, gate, notenum, 0, 0);

	// mapのキーは128（ノートナンバー）* 分解能（480）* 4 * 128をしてもint型に余裕で収まる
	INT_MAX;
	2147483647;
}

void MidiEventManager::autoCreate() {
	// とりあえず、自動作曲の伴奏部分を作ってみる。
	// ランダムで配置
	static const int majorScale[8] = { 60, 62, 64, 65, 67, 69, 71, 72 };
	static const int chordC[4] = { 60, 64, 67, 72 };
	for (int i = 0; i < 10; i++)
		addNote(activeCh, 0, majorScale[GetRand(7)], div / 8);
}

void MidiEventManager::draw() {
	// MIDIイベントを表示（現在操作中のチャンネルのNoteONイベントのみ）
	int j = 0;
	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		if (itr->GetVel() == 0) continue;
		DrawFormatString(0, 20 * j, WHITE, "CH:%d Delta:%d Note:%d Gate:%d", itr->GetCh(), itr->GetDelta(), itr->GetNote(), itr->GetGate());
		j++;
	}
}

int MidiEventManager::getMidiMsgForSMF(char* data) {
	int i = 0;
	// 最初の処理

	for (auto itr = note.cbegin(); itr != note.cend(); itr++) {
		// デルタタイムの計算
		int delta = itr->GetDelta();
		if (delta >> 21 != 0) data[i++] = (delta >> 21) | 128;
		if (delta >> 14 != 0) data[i++] = ((delta >> 14) & 127) | 128;
		if (delta >> 7 != 0) data[i++] = ((delta >> 7) & 127) | 128;
		data[i++] = delta & 127;

		data[i++] = (9 << 4) | itr->GetCh(); // 第一バイト
		data[i++] = itr->GetNote(); // 第二バイト
		data[i++] = itr->GetVel(); // 第三バイト
	}

	// メタイベント・EOF（00 FF 2F 00）
	data[i++] = 0x00; data[i++] = 0xFF; data[i++] = 0x2F; data[i++] = 0x00;
	return i;
}