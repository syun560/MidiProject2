#include "Conducter.h"

Conducter::Conducter() {
	pretick = tick = 0.0;
	mea = 1;
	delta = 12.0;
	bpm = 120.0;
	Resolution = 480.0;
	Beat = 4;
	now = 0;
	bit = Resolution / 4.0;
	state = PAUSE;
	Repeat = false;
	Key = "C";
}

int Conducter::Update() {
	delta = bpm * Resolution / 3600.0; // delta(1フレームでどれくらい進めばよいか）をBPMから計算
	if (state != PLAYING) return -1;
	pretick = tick;
	tick += delta;
	//一小節超えたら
	if (tick >= Resolution * Beat) {
		tick -= Resolution * Beat;
		mea++;
		if (Repeat) mea--;
		// return 0;
	}
	// 普通に剰余演算で良いのでは？→変わった瞬間を知るためかな、商でないとだめ
	//for (int i = 0; i < 16; i++) {
	//	if (pretick <= bit*i && bit*i < tick) {
	//		return i;
	//	}
	//}
	//return (int)tick%(int)bit;
	return tick + mea * Resolution * Beat;
}

void Conducter::Slower() {
	bpm -= 1.0;
	if(bpm == 29) bpm = 30;
}

void Conducter::SetRepeat() {
	Repeat == true ? Repeat = false: Repeat = true;
}

void Conducter::Faster() {
	bpm += 1.0;
	if (bpm == 301) bpm = 300;
}

void Conducter::NextMea() {
	if(++mea == 126) mea = 125;
}

bool Conducter::PreMea() {
	if (--mea == -1) {
		mea = 0;
		return false;
	}
	else return true;
}

void Conducter::FirstBeat() {
	if (tick == 0.0) PreMea();
	else tick = 0.0;
}

void Conducter::EndBeat() {
	if (tick != 0.0) FirstBeat();
	NextMea();
}

void Conducter::Stop() {
	mea = 1;
	pretick = tick = 0.0;
}

void Conducter::PlayOrPause() {
	if (state == PLAYING) {
		state = PAUSE;
	}
	else state = PLAYING;
}

void Conducter::Draw(int x, int y) const{
	if(Repeat) DrawCircle(20, 20, 10, RED, TRUE);
	DrawFormatString(10, 10, WHITE, "%2d:%4.0f bpm:%0.f Key:%s", mea, tick, bpm, Key);
	for (int i = 0; i < 16; i++) {
		if (i == int(tick / 120.0)) {
			DrawCircle(i * 16 + 16 / 2 + x, y - 16 / 2, 3, RED);
		}
		else {
			DrawCircle(i * 16 + 16 / 2 + x, y - 16 / 2, 3, RED, FALSE);
		}
	}
}

double Conducter::GetDelta() const {
	return delta;
}

double Conducter::GetPreTick() const {
	return pretick + mea * Resolution * Beat;
}

int Conducter::GetMea() const {
	return mea;
}

void Conducter::Save(FILE* fp) {
	fwrite(&bpm, sizeof(double), 1, fp);
}

void Conducter::Load(FILE* fp) {
	fread(&bpm, sizeof(double), 1, fp);
}