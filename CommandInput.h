#pragma once
#include "global.h"

class CommandInput {
	char Message[100];
	int InputHandle;
	bool InputMode;
public:
	CommandInput();
	~CommandInput();
	void TurnInputMode();
	char* Update();
	void Draw(int x, int y) const;
};