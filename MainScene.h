#pragma once
#include "global.h"
#include "smfIO.h"
#include "MidiEventManager.h"

class MainScene {
	smfIO smfio; // SMFの読み書き
	MidiEventManager midiEventManager; // MIDIイベントの管理
public:
	MainScene();
	void Update();
	void Draw();
};