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
	int Update();
	void FocusUp();
	void FocusDown();
	void ValueUp();
	void ValueDown();
	void Draw(int x, int y) const;
	int GetFocus() const;
};