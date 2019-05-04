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
	if (state != PLAYING) return -1;
	pretick = tick;
	delta = bpm * Resolution / 3600.0;
	tick += delta;
	//�ꏬ�ߒ�������
	if (tick >= Resolution * Beat) {
		tick -= Resolution * Beat;
		mea++;
		if (Repeat) mea--;
		return 0;
	}
	for (int i = 0; i < 16; i++) {
		if (pretick <= bit*i && bit*i < tick) {
			return i;
		}
	}
	return -1;
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

void Conducter::PreMea() {
	if (--mea == -1) mea = 0;
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

void Conducter::Pause() {
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

int Conducter::GetMea() const {
	return mea;
}

void Conducter::Save(FILE* fp) {
	fwrite(&bpm, sizeof(double), 1, fp);
}

void Conducter::Load(FILE* fp) {
	fread(&bpm, sizeof(double), 1, fp);
}