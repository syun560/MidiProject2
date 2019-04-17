#pragma once
#include "global.h"
#include "smfIO.h"
#include "MidiEventManager.h"

class MainScene {
	smfIO smfio; // SMF�̓ǂݏ���
	MidiEventManager midiEventManager; // MIDI�C�x���g�̊Ǘ�
public:
	MainScene();
	void Update();
	void Draw();
};