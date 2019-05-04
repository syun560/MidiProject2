#pragma once
#include "global.h"
#include "smfIO.h"
#include "MidiEventManager.h"

#include "MidiController.h"
#include "Conducter.h"

class MainScene {
	smfIO smfio; // SMFの読み書き
	MidiEventManager midiEventManager; // MIDIイベントの管理
	MidiController midiController;
	Conducter conductor;
	int font;
public:
	MainScene();
	void Update();
	void Draw();
};