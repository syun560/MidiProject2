#include "CommandInput.h"

CommandInput::CommandInput() {
	InputHandle = MakeKeyInput(100, TRUE, TRUE, FALSE);
	memset(Message, 0, 100);
	InputMode = false;
}

CommandInput::~CommandInput() {
	DeleteKeyInput(InputHandle);
}

void CommandInput::TurnInputMode() {
	SetActiveKeyInput(InputHandle);
	SetKeyInputString("", InputHandle);
	InputMode = true;
}

char* CommandInput::Update() {
	// ���͂��m�肵�����̏���
	int state = CheckKeyInput(InputHandle);
	if (state != 0) { //0:�܂����� 1:���� 2:�L�����Z�� -1:�G���[
		GetKeyInputString(Message, InputHandle);
		InputMode = false;
		return Message;
	}
	return NULL;
}

void CommandInput::Draw(int x, int y) const{
	DrawKeyInputModeString(FMX-40, FMX - 20);
	DrawKeyInputString(x, y, InputHandle);
}