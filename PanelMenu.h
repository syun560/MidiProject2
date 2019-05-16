#pragma once
#include "global.h"
#include <vector>

class PanelMenu {
	int num;
	int focus;
	std::vector<char*> panel;
	bool active;
public:
	PanelMenu();
	void Set(int num, char* name);
	void FocusUp();
	void FocusDown();
	void Draw(int x, int y) const;
	int GetFocus() const;
};