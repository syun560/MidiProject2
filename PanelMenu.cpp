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

int PanelMenu::Update() {
	if (!active) return 0;
	static const int holdtime = 30;
	if (Input::Key(KEY_INPUT_LSHIFT) < 1) return 0;
	if (Input::Key(KEY_INPUT_DOWN) == 1 || Input::Key(KEY_INPUT_DOWN) > holdtime) {
		if (++focus == num) focus = 0;
	}
	if (Input::Key(KEY_INPUT_UP) == 1 || Input::Key(KEY_INPUT_UP) > holdtime) {
		if (--focus == -1) focus = num - 1;
	}
	if (Input::Key(KEY_INPUT_RIGHT) == 1 || Input::Key(KEY_INPUT_RIGHT) > holdtime) {
		return focus+1;
	}
	if (Input::Key(KEY_INPUT_LEFT) == 1 || Input::Key(KEY_INPUT_LEFT) > holdtime) {
		return -1 * (focus+1);
	}
	return 0;
}

void PanelMenu::FocusUp() {

}
void PanelMenu::FocusDown() {

}
void PanelMenu::ValueUp() {

}
void PanelMenu::ValueDown() {

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