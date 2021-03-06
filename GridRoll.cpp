#include "GridRoll.h"
#include "Input.h"


GridRoll::GridRoll() {
	BlockSize = 16;
	BlockWidth = 16;
	BlockHeight = 12;
	DeleteAll();
	Base = 60;
	Ch = Mea = 0;
	x = 50;
	y = 50;
}

// そのチャンネル内のMidiメッセージを一意に扱うためのidを作る
int getID(int Mea, int bit, int note) {
	int Reso = 120; // 分解能
	return (Mea * 16 + bit) * 128 * Reso + note;
}

void GridRoll::SetMidiEvent(int bit, int note, int val) {
	int gate;
	// Noteが無い場所にはGateを登録しない
	switch (val) {
	case -1:
		gate = -1;
		break;
	case 0:// 全音符
		gate = 1920;
		break;
	case 1:// 2分音符
		gate = 960;
		break;
	case 2:// 4分音符
		gate = 480;
		break;
	case 3:// 8分音符
		gate = 240;
		break;
	case 4:// 16分音符
		gate = 120;
		break;
	}
	int id = getID(Mea, bit, note);
	auto itr = midiEvent[Ch].find(id);
	if (itr != midiEvent[Ch].end()) { // 登録されている場合の処理
		return;
	}
	else { // 登録されていない場合の処理
		midiEvent[Ch][id] = MidiEvent(Ch, Mea, bit, note, gate, 100);
	}	
}

void GridRoll::Update(int mea, int ch) {
	Mea = mea;
	Ch = ch;
	static int mx, my;
	int dx = 50;
	int dy = 50;
	Input::Mouse(&mx, &my);
	// 左クリックで登録
	//if (Input::MouseL() > 0) {
	//	int ex = (mx - dx) / BlockSize;
	//	int ey = BlockHeight - (my - dy) / BlockSize;
	//	int eey = (my - dy) / BlockSize;
	//	if (0 <= ex && ex < BlockWidth) {
	//		// 上のGrid
	//		if (0 <= ey && ey <= BlockHeight) {
	//			SetMidiEvent(ex, ey + Base, 4);
	//		}
	//		// 下のGrid
	//		/*else if (BlockHeight+1 < eey && eey < BlockHeight + 7) {
	//			SetMidiEvent(ex, eey - BlockHeight - 2);
	//		}*/
	//	}
	//}
	// 右クリックで削除
	/*if (Input::MouseR() > 0) {
		int ex = (mx - dx) / BlockSize;
		if (ex < 0 || ex > BlockWidth) {}
		else {
			Note[Ch][Mea][ex] = -1;
			TransRealGate(ex, -1);
		}
	}*/
}

void GridRoll::Save(FILE *fp) {
	
}

void GridRoll::Load(FILE *fp) {
	
}

void GridRoll::DeleteOnePhrase() {
	midiEvent[Ch].clear();
}

void GridRoll::DeleteAll() {
	for (int i = 0; i < 16; ++i) {
		midiEvent[i].clear();
	}
}

void GridRoll::Copy() {
	
}

void GridRoll::Paste() {
	
}

void GridRoll::Draw() const{
	// Noteを描画
	//int beginid = getID(Mea, 0, 0);
	//int endid = getID(Mea + 1, 0, 0);
	//for (int i = beginid; i < endid; ++i) {
	//	auto itr = midiEvent[Ch].find(i);
	//	if (itr != midiEvent[Ch].end()) { // 登録されている場合
	//		int n = itr->second.GetNote() - Base; // Baseから何個上の音か？
	//		if (0 <= n && n < BlockHeight) {
	//			int x1 = itr->second.GetBit() * BlockSize + x;
	//			int y1 = itr->second.GetBit() * BlockSize + y;
	//			double x2 = (double)x1 + BlockSize * itr->second.GetGate() / 120.0;
	//			int y2 = y1 + BlockSize;
	//			DrawEdgeBox(x1, y1, (int)x2, y2, GREEN);
	//		}
	//	}
	//}
	


	/*for (int i = 0; i < BlockWidth; i++) {
		for (int j = 0; j <= BlockHeight; j++) {
			int n = me[Ch][0].GetNote() - Base;
			if (n == BlockHeight - j) {
				int x1 = i * BlockSize + x;
				int y1 = j * BlockSize + y;
				double x2 = (double)x1 + BlockSize * RealGate[Ch][Mea][i] / 120.0;
				int y2 = y1 + BlockSize;
				MyDx::DrawEdgeBox(x1, y1, (int)x2, y2, MyDx::GREEN);
			}
		}
	}*/

	// Gateを描画
	/*int ky = y + (BlockHeight+2) * BlockSize;
	for (int i = 0; i < BlockWidth; i++) {
		for (int j = 0; j < 5; j++) {
			if (Gate[Ch][Mea][i] == j) {
				MyDx::DrawEdgeBox(i * BlockSize + x, j * BlockSize + ky, (i + 1) * BlockSize + x, (j + 1) * BlockSize + ky, gatecol, MyDx::WHITE);
			}
			else DrawBox(i * BlockSize + x, j * BlockSize + ky, (i + 1)*BlockSize + x, (j + 1)*BlockSize + ky, MyDx::WHITE, FALSE);
		}
	}*/

	// MidiEvent情報
	int i = 0;
	for (auto itr = midiEvent[Ch].begin(); itr != midiEvent[Ch].end(); itr++) {
		itr->second.Draw(FMX - 200, 50 + 20*i);
		i++;
	}
}