#include "MainScene.h"
#include "Input.h"

MainScene::MainScene() {
	font = CreateFontToHandle(NULL, 40, 3, DX_FONTTYPE_ANTIALIASING);
}

void MainScene::Update() {
	// ����n
	if (Input::Key(KEY_INPUT_O) == 1) { // �t�@�C���ǂݍ���
		if (smfio.read("input.mid") == -1) printfDx("�ǂݍ��ݎ��s\n");
		else printfDx("�ǂݍ��ݐ���\n");
	}
	if (Input::Key(KEY_INPUT_S) == 1) { // �t�@�C�������o��
		char data[10000]; // TODO �ł���Γ��I�Ɍ��肷�ׂ�
		int size = midiEventManager.getMidiMsgForSMF(data);
		if (smfio.write("output.mid", data, size) == -1) printfDx("�������ݎ��s\n");
		else printfDx("�������ݐ���\n");
	}
	if (Input::Key(KEY_INPUT_A) == 1) { // �Đ�
		PlayMusic("output.mid", DX_PLAYTYPE_BACK);
		printfDx("%s���Đ�\n", "output.mid");
	}
	if (Input::Key(KEY_INPUT_J) == 1) { // �������
		midiEventManager.autoCreate(480 * 3 * 32);
	}

	if (Input::Key(KEY_INPUT_D) == 1) StopMusic(); // ��~

	// �f�o�b�O�n
	if (Input::Key(KEY_INPUT_C) == 1) {
		clsDx();
		midiEventManager.deleteAllEvent();
	}

	midiController.Update(conductor.Update());
}

void MainScene::Draw(){
	// MIDI�C�x���g��\��
	midiEventManager.draw();
	midiController.Draw();

	// ������@��\��
	if (Input::Key(KEY_INPUT_M) > 0) {
		static const int INFO_X = 400;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(INFO_X - 20, 80, FMX - 20, 240, BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawFormatString(INFO_X, 80, WHITE, "������@\nO�L�[�œǍ�\nS�L�[�ŕۑ�\nA�L�[�ōĐ�\nD�L�[�Œ�~\nC�L�[�ŃN���A\nESC�ŏI��");
	}
	

	// TODO:�e���|��v���C�󋵁A�X�P�[����\��
	// �R�[�h��\��
	DrawStringToHandle(FMX/2 - 30, FMY/2 - 30, "Dm", WHITE, font);
}