#pragma once
#include "global.h"
#include "smfIO.h"
#include "MidiEventManager.h"
#include "CommandInput.h"
#include "MidiController.h"
#include "Conducter.h"
#include "GridRoll.h"

class MainScene {
	smfIO smfio; // SMFの読み書き
	MidiEventManager midiEventManager; // MIDIイベントの管理
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
	int rootNote, baseNote;

	// デバッグ用
	int key1, key2;
	int tenkai(int key);
public:
	MainScene();
	int Update();
	void Draw();
};