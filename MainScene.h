#pragma once
#include "global.h"
#include "smfIO.h"
#include "MidiEventManager.h"

#include "MidiController.h"
#include "Conducter.h"

class MainScene {
	smfIO smfio; // SMF�̓ǂݏ���
	MidiEventManager midiEventManager; // MIDI�C�x���g�̊Ǘ�
	MidiController midiController;
	Conducter conductor;
	int font;
public:
	MainScene();
	void Update();
	void Draw();
};