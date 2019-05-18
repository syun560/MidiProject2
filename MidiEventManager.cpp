#include "MidiEventManager.h"

static const char keyName[12][4] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
static const int ScaleC[13] = { 1,1,0,1,0,1,0,1,1,0,1,0,1 };
static const int avoidcol = GetColor(80, 80, 80);
static const int gatecol = GetColor(200, 200, 255);
static const int basecol = GetColor(80, 180, 202);

MidiEventManager::MidiEventManager() {
	div = 480;
	activeCh = 0;
	BlockSize = 16;
	BlockWidth = 16;
	BlockHeight = 24;
	Base = 60;
	// グリッドの位置
	x = 50;
	y = 50;
}

void MidiEventManager::addNote(int ch, int delta, int notenum, int gate, int vel) {
	// 1拍ぶんの休符を追加したい
	//if (seq == 0) {
	//	note[ch].emplace_back(ch, div * 4, 60, 0, 0);
	//	seq += div * 4;
	//	return;
	//}

	//// ノートオンとノートオフ(vel0)を追加
	//note[ch].emplace_back(ch, delta, notenum, gate, vel);
	//note[ch].emplace_back(ch, gate, notenum, 0, 0);

	// mapのキーは分解能（480）* 拍（4）小節（128)をしてもint型に余裕で収まる
	// INT_MAX = 2147483647;
	int mapKey1 = seq + delta;
	int mapKey2 = seq + delta + gate - 1; // NoteOffのキーは次のNoteOnと重ならないように-1する
	noteMap[ch].emplace(mapKey1, NoteOnEvent(ch, delta, notenum, gate, vel));
	noteMap[ch].emplace(mapKey1, NoteOnEvent(ch, gate, notenum, 0, 0));

	// 現在のシーケンス位置を移動
	seq += delta + gate;
}

void MidiEventManager::autoCreate(int length) {
	seq = 0;

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
	noteMap[activeCh].clear();
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

void MidiEventManager::Update(int focusch, int dmea) {
	activeCh = focusch;
	mea = dmea;
}

void MidiEventManager::keyDown() {
	for (auto itr = noteMap[activeCh].begin(); itr != noteMap[activeCh].end(); itr++) {
		itr->second.transpose(-12);
	}
}

void MidiEventManager::keyUp() {
	for (auto itr = noteMap[activeCh].begin(); itr != noteMap[activeCh].end(); itr++) {
		itr->second.transpose(12);
	}
}

void MidiEventManager::draw() {
	// Base音を表示
	DrawFormatString(x - 30, y + BlockHeight * BlockSize, WHITE, "C%d", Base / 12);
	DrawFormatString(x - 30, y + BlockHeight * BlockSize / 2, WHITE, "C%d", Base / 12 + 1);
	DrawFormatString(x - 30, y, WHITE, "C%d", Base / 12 + 2);

	// グリッドを描画
	for (int i = 0; i < BlockWidth; i++) {
		for (int j = 0; j <= BlockHeight; j++) {
			if (j % 12 == 0) DrawEdgeBox(i * BlockSize + x, j * BlockSize + y, (i + 1)*BlockSize + x, (j + 1)*BlockSize + y, basecol, avoidcol);
			else if (ScaleC[j % 12] == 0) DrawEdgeBox(i * BlockSize + x, j * BlockSize + y, (i + 1)*BlockSize + x, (j + 1)*BlockSize + y, BLACK, avoidcol);
			else DrawEdgeBox(i * BlockSize + x, j * BlockSize + y, (i + 1)*BlockSize + x, (j + 1)*BlockSize + y, WHITE, avoidcol
			);
		}
	}

	// MIDIイベントを表示（現在操作中のチャンネルのNoteONイベントのみ）
	int j = 0;
	// Condoctorの動きに合わせて表示するようにしたい。
	int firsttick = mea * div * 4;
	int endtick = (mea + 1) * div * 4 - 1;
	for (auto itr = noteMap[activeCh].lower_bound(firsttick); itr != noteMap[activeCh].upper_bound(endtick); itr++) {
		if (itr->second.GetVel() == 0) continue;
		int tick = itr->first % (div * 4);
		int delta = itr->second.GetDelta();
		int note = itr->second.GetNote();
		int gate = itr->second.GetGate();
		DrawFormatString(540, 50 + 20 * j, WHITE, "%5d %2s%d(%d) %d", tick, keyName[note % 12], note / 12, note, gate);
		if (++j == 20) break;

		// Noteを描画
		int n = note - Base; // Baseから何個上の音か？
		if (0 <= n && n < BlockHeight) {
			int x1 = BlockSize * tick / 120.0 + x;
			int y1 = (BlockHeight - n) * BlockSize + y;
			int x2 = x1 + BlockSize * gate / 120.0;
			int y2 = y1 + BlockSize;
			DrawEdgeBox(x1, y1, x2, y2, GREEN);
		}


	}
	DrawBox(540, 50, 740, 50 + 20 * 20, GREEN, FALSE);
	// DrawBox(540, 50, 740, 50 + 20, YELLOW, FALSE);
}

void MidiEventManager::LowerOctave() {
	Base -= 12;
	if (Base < 0) Base += 12;
}

void MidiEventManager::HigherOctave() {
	Base += 12;
	if (Base > 127) Base -= 12;
}

int MidiEventManager::getMidiMsgForSMF(char* data) {
	int i = 0;
	// 最初の処理

	for (auto itr = noteMap[activeCh].cbegin(); itr != noteMap[activeCh].cend(); itr++) {
		// デルタタイムの計算
		int delta = itr->second.GetDelta();
		if (delta >> 21 != 0) data[i++] = (delta >> 21) | 128;
		if (delta >> 14 != 0) data[i++] = ((delta >> 14) & 127) | 128;
		if (delta >> 7 != 0) data[i++] = ((delta >> 7) & 127) | 128;
		data[i++] = delta & 127;

		data[i++] = (9 << 4) | itr->second.GetCh(); // 第一バイト
		data[i++] = itr->second.GetNote(); // 第二バイト
		data[i++] = itr->second.GetVel(); // 第三バイト
	}

	// メタイベント・EOF（00 FF 2F 00）
	data[i++] = 0x00; data[i++] = 0xFF; data[i++] = 0x2F; data[i++] = 0x00;
	return i;
}