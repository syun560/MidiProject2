#include "MidiController.h"

const static char InstrumentsName[128][32] = {
	"Piano 1",
	"Piano 2",
	"Piano 3",
	"Honkey-tonk",
	"E.Piano 1",
	"E.Piano 2",
	"Harpsichord",
	"Clav.",
	"Celesta",
	"Glockenspiel",
	"Music Box",
	"Vibraphone",
	"Marimba",
	"Xylophone",
	"Tubular Bells",
	"Dulcrimer",
	"Organ 1",
	"Organ 2",
	"Organ 3",
	"Pipe Organ",
	"Reed Organ",
	"Accordion",
	"Harmonica",
	"Bandneon",
	"Nylon-str.Guitar",
	"Steel-str.Gt",
	"Jazz Guitar",
	"Clean Guitar",
	"Muted Guitar",
	"Overdrive Guitar",
	"Distortion Guitar",
	"Gt.Harmonics",
	"Acoustic Bass",
	"Fingered Bass",
	"Picked Bass",
	"Fretless Bass",
	"Slap Bass 1",
	"Slap Bass 2",
	"Synth Bass 1",
	"Synth Bass 2",
	"Violin",
	"Viola",
	"Cello",
	"Contrabass",
	"Tremolo Strings",
	"Pizzicato Strings",
	"Harp",
	"Timpani",
	"Strings",
	"Slow Strings",
	"Synth Strings 1",
	"Synth Strings 2",
	"Chor Aahs",
	"Voice Oohs",
	"Synth Vox",
	"OrchestraHit",
	"Trumpet",
	"Trombone",
	"Tuba",
	"MutedTrumpet",
	"French Horns",
	"Brass 1",
	"Synth Brass 1",
	"Synth Brass 2",
	"Soprano Sax",
	"Alto Sax",
	"Tenor Sax",
	"Baritone Sax",
	"Oboe",
	"English Horn",
	"Bassoon",
	"Clarinet",
	"Piccolo",
	"Flute",
	"Recorder",
	"Pan Flute",
	"Bottle Blow",
	"Shakuhachi",
	"Whistle",
	"Ocarina",
	"Square Wave",
	"Saw Wave",
	"Synth Calliope",
	"Chiffer Lead",
	"Charang",
	"Solo Vox",
	"5th Saw Wave",
	"Bass & Lead",
	"Fantasia",
	"Warm Pad",
	"Polysynth",
	"Space Voice",
	"Bowed Glass",
	"Metal Pad",
	"Halo Pad",
	"Sweep Pad",
	"Ice Rain",
	"SoundTrack",
	"Crystal",
	"Atomosphere",
	"Brightness",
	"Goblin",
	"Echo Drops",
	"Star Theme",
	"Sitar",
	"Banjo",
	"Shamisen",
	"Koto",
	"Kalimba",
	"Bagpipe",
	"Fiddle",
	"Shenai",
	"Tinkle Bell",
	"Agogo",
	"Steel Drums",
	"Woodblock",
	"Taiko",
	"Melodic Tom",
	"Synth Drum",
	"Reverse Sym",
	"Gt.FretNoise",
	"Breath Noise",
	"Seashore",
	"Bird",
	"Telephone",
	"Helicopter",
	"Applause",
	"Gun Shot",
};
const static char DrumName[128][32] = {
	"STANDARD1",
	"STANDARD2",
	"STANDARD3",
	"",
	"",
	"",
	"",
	"ROOM",
	"HIP HOP",
	"JUNGLE",
	"TECHNO",
	"--------------",
	"HOUSE",
	"",
	"",
	"POWER",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"ELECTRONIC",
	"TR-808",
	"DANCE",
	"CR-78",
	"",
	"TR-707",
	"TR-909",
	"",
	"JAZZ",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"BRUSH",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"ORCHESTRA",
	"ETHNIC",
	"KICK & SNARE",
	"",
	"",
	"",
	"",
	"",
	"SFX",
};


MidiController::MidiController() :CH_MAX(16){
	for (int i = 0; i < CH_MAX; i++) NowProgram[i] = 0;
	panel.Set(16,"");
}

int MidiController::Init() {
	// MIDIデバイスオープン
	if (midiOutOpen(&g_hMidi, MIDIMAPPER, 0, 0, 0) != MMSYSERR_NOERROR) {
		return -1;
	}

	//メモリ初期化
	ZeroMemory(&mhMidi, sizeof(mhMidi));
	
	//GMリセット送信用バッファ設定
	mhMidi.lpData = (LPSTR)abyGMReset;
	mhMidi.dwBufferLength = 6;
	mhMidi.dwBytesRecorded = 6;
	midiOutPrepareHeader(g_hMidi, &mhMidi, sizeof(mhMidi));

	//GMリセットメッセージ送信
	if (midiOutLongMsg(g_hMidi, &mhMidi, sizeof(mhMidi)) != MMSYSERR_NOERROR) {
		midiOutUnprepareHeader(g_hMidi, &mhMidi, sizeof(mhMidi));
		MessageBox(NULL, "MIDI音源の初期化に失敗しました。", "初期化失敗", MB_OK);
		midiOutClose(g_hMidi);
		return -1;
	}

	//GMリセット完了待機
	while ((mhMidi.dwFlags & MHDR_DONE) == 0);
	midiOutUnprepareHeader(g_hMidi, &mhMidi, sizeof(mhMidi));

	return 0;
}

void MidiController::Update(double delta) {
	//鳴っている音の管理（削除）
	auto itr = cs.begin();
	while (itr != cs.end()) {
		if (itr->Update(delta) == -1) { // GateRemainが0以下になったら鳴っている音を消去
			Stop(itr->GetCh(), itr->GetNote());//TODO 問題点、次のNoteOnメッセージが近いと次の音まで止めてしまう。
			itr = cs.erase(itr);
		}
		else ++itr;
	}

	//パネル操作、ProgramChangeなど
	int p = panel.Update();
	if (p > 0) {
		ProgramChange(p - 1, NowProgram[p-1] + 1);
	}
	else if (p < 0) {
		p *= -1;
		ProgramChange(p - 1, NowProgram[p-1] - 1);
	}
}

void MidiController::NextProgram() {

}

void MidiController::PreProgram() {

}

void MidiController::NextCh() {

}

void MidiController::PreCh() {

}

// 鳴る音の登録
void MidiController::Play(int ch,int note,int gate, int vel) {
	//DWORD AllNoteOff = 0x7B << 8 | 0xB << 4 | ch;
	//midiOutShortMsg(g_hMidi, AllNoteOff);
	// すでに同じ音が登録されていたら、音を止め、音を削除し、その音を流す処理を入れる
	for (auto itr = cs.cbegin(); itr != cs.cend(); itr++) {
		if (itr->IsCollide(ch, note)) {
			Stop(ch, note);
			itr = cs.erase(itr);
			break;
		}
	}
	DWORD NoteOn = vel << 16 | note << 8 | 9 << 4 | ch;
	midiOutShortMsg(g_hMidi, NoteOn);
	cs.emplace_back(ch, note, gate);
}


void MidiController::Stop(int ch, int note) {
	DWORD NoteOff = 0 << 16 | note << 8 | 9 << 4 | ch;
	midiOutShortMsg(g_hMidi, NoteOff);
}

void MidiController::AllStop() {
	for (int i = 0; i < CH_MAX; i++) {
		DWORD NoteOff = 0x7B << 8 | 0xB << 4 | i;
		midiOutShortMsg(g_hMidi, NoteOff);
	}
}

void MidiController::ProgramChange(int ch, int num) {
	if (ch < 0 || ch >= 16 || num < 0 || num >= 128) return;
	DWORD NoteOn = num << 8 | 0xC << 4 | ch;
	midiOutShortMsg(g_hMidi, NoteOn);
	NowProgram[ch] = num;
}

MidiController::~MidiController() {
	midiOutReset(g_hMidi);
	midiOutClose(g_hMidi);
}

void MidiController::Draw() const{
	for (int i = 0; i < 16; i++) {
		if(i==9) DrawFormatString(FMX / 2, 50 + 20 * i, WHITE, "%s (%d)", DrumName[NowProgram[i]], NowProgram[i]);
		else DrawFormatString(FMX / 2, 50 + 20*i, WHITE, "%s (%d)", InstrumentsName[NowProgram[i]], NowProgram[i]);
	}
	// 鳴っている音を表示する
	int j = 18;
	for (auto itr = cs.cbegin(); itr != cs.cend(); itr++) {
		DrawFormatString(50, j * 20, WHITE, "CH:%d Note:%d Gate:%.2f", itr->GetCh(), itr->GetNote(), itr->GetGate());
		j++;
	}
	panel.Draw(FMX / 2, 50);
}

void MidiController::Save(FILE* fp) {
	fwrite(NowProgram, sizeof(int), CH_MAX, fp);
}

void MidiController::Load(FILE* fp) {
	fread(NowProgram, sizeof(int), CH_MAX, fp);
	for (int i = 0; i < CH_MAX; ++i) ProgramChange(i, NowProgram[i]);
}

int MidiController::GetFocusCh() const{
	return panel.GetFocus();
}