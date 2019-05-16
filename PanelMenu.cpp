#include "PanelMenu.h"
#include "Input.h"

PanelMenu::PanelMenu() {
	num = 0;
}

void PanelMenu::Set(int dnum, char* name) {
	num = dnum;
	for (int i = 0; i < num; i++) {
		panel.emplace_back(name);
	}
	active = true;
	focus = 0;
}

void PanelMenu::FocusUp() {
	if (++focus == num) focus = 0;
}
void PanelMenu::FocusDown() {
	if (--focus == -1) focus = num - 1;
}

void PanelMenu::Draw(int x, int y) const{
	DrawBox(x,y,x+150,y+num*20,GREEN,FALSE);
	for (int i = 0; i < num; i++) {
		if (i == focus) DrawBox(x, y +i * 20, x + 150, y+i*20+20,YELLOW,FALSE);
		DrawFormatString(x, y+i*20, WHITE, "%s", panel[i]);
	}
}

int PanelMenu::GetFocus() const{
	return focus;
}