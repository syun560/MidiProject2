#pragma once
#include "global.h"
#include "smfIO.h"
#include "MidiEventManager.h"
#include "CommandInput.h"
#include "MidiController.h"
#include "Conducter.h"
#include "GridRoll.h"

class MainScene {
	smfIO smfio; // SMF�̓ǂݏ���
	MidiEventManager midiEventManager; // MIDI�C�x���g�̊Ǘ�
	MidiController midiController;
	Conducter conductor;
	GridRoll gridRoll;
	CommandInput commandInput;
	bool InputMode;
	int font;
	int ExecCommand(const char* command);
	char FilePath[100];
	void FileOpen(const char* filename);
	void FileSave(const char* filename);
	bool legacyMode;
public:
	MainScene();
	int Update();
	void Draw();
};