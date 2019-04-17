#pragma once
#include "global.h"
#include "smfIO.h"
#include "MidiEventManager.h"

class MainScene {
	smfIO smfio; // SMF‚Ì“Ç‚İ‘‚«
	MidiEventManager midiEventManager; // MIDIƒCƒxƒ“ƒg‚ÌŠÇ—
public:
	MainScene();
	void Update();
	void Draw();
};