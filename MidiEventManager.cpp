#include "MidiEventManager.h"

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 1;
}

void MidiEventManager::addNote(int ch, int delta, int notenum, int gate, int vel) {
	// 1拍ぶんの休符を追加したい
	if (seq == 0) {
		note[ch].emplace_back(ch, div * 4, 60, 0, 0);
		seq += div * 4;
		return;
	}

	// ノートオンとノートオフ(vel0)を追加
	note[ch].emplace_back(ch, delta, notenum, gate, vel);
	note[ch].emplace_back(ch, gate, notenum, 0, 0);

	// mapのキーは分解能（480）* 拍（4）小節（128)をしてもint型に余裕で収まる
	// INT_MAX = 2147483647;
	int mapKey1 = seq + delta;
	int mapKey2 = seq + delta + gate - 1; // NoteOffのキーは次のNoteOnと重ならないように-1する
	noteMap[ch].insert(std::pair<int, NoteOnEvent>(mapKey1, NoteOnEvent(ch, delta, notenum, gate, vel)));
	noteMap[ch].insert(std::pair<int, NoteOnEvent>(mapKey2, NoteOnEvent(ch, gate, notenum, 0, 0)));
	
	// 現在のシーケンス位置を移動
	seq += delta + gate;
}

void MidiEventManager::autoCreate(int length) {
	// とりあえず、自動作曲の伴奏部分を作ってみる。
	// ランダムで配置（あまりに飛び飛びの演奏はやめる）
	static const int majorScale[8] = { 60, 62, 64, 65, 67, 69, 71, 72 };
	static const int chordC[4] = { 60, 64, 67, 72 };
	static const int kokken[] = { 54, 56, 58, 61, 63, 66, 68, 70, 73, 75, 78 };
	static const int gate[8] = { div / 2 , div / 2, div / 2, div / 2, div, div, div, div * 3 / 2};
	static const int rest[] = { 0, 0, 0, 0, 0, div / 2};
	int key, preKey;
	key = preKey = GetRand(10);
	while (seq < length) {
		preKey = key; // 前に押した黒鍵を覚えておく
		do {
			key = preKey - 2 + GetRand(4);
		} while (key < 0 || key > 10);
		addNote(activeCh, rest[GetRand(5)], kokken[key], gate[GetRand(7)], 90 + GetRand(20));
	}	
}

void MidiEventManager::deleteAllEvent() {
	note[activeCh].clear();
	seq = 0;
}

int MidiEventManager::GetNoteData(int ch, int key) const {
	auto itr = noteMap[ch].find(key);
	if (itr != noteMap[ch].end()) {
		return itr->second.GetNote();
	}
	else return -1;
}
int MidiEventManager::GetGateData(int ch, int key) const {
	auto itr = noteMap[ch].find(key);
	if (itr != noteMap[ch].end()) {
		return itr->second.GetGate();
	}
	else return -1;
}
int MidiEventManager::GetVelData(int ch, int key) const {
	auto itr = noteMap[ch].find(key);
	if (itr != noteMap[ch].end()) {
		return itr->second.GetVel();
	}
	else return -1;
}


void MidiEventManager::draw() {
	// MIDIイベントを表示（現在操作中のチャンネルのNoteONイベントのみ）
	int j = 0;
	for (auto itr = note[activeCh].cbegin(); itr != note[activeCh].cend(); itr++) {
		if (itr->GetVel() == 0) continue;
		DrawFormatString(100, 20 * j, WHITE, "CH:%d Delta:%d Note:%d Gate:%d", itr->GetCh(), itr->GetDelta(), itr->GetNote(), itr->GetGate());
		j++;
	}
	/*int j = 0;
	for (auto itr = noteMap[activeCh].cbegin(); itr != noteMap[activeCh].cend(); itr++) {
		if (itr->second.GetVel() == 0) continue;
		DrawFormatString(100, 20 * j, WHITE, "mapKey:%d CH:%d Delta:%d Note:%d Gate:%d", itr->first, itr->second.GetCh(), itr->second.GetDelta(), itr->second.GetNote(), itr->second.GetGate());
		j++;
	}*/
}

int MidiEventManager::getMidiMsgForSMF(char* data) {
	int i = 0;
	// 最初の処理

	for (auto itr = note[activeCh].cbegin(); itr != note[activeCh].cend(); itr++) {
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