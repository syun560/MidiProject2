#include "MainScene.h"
#include "Input.h"

static unsigned char rest[5] = {0x8F, 0x00, 0x80, 0x30, 0x00};

MainScene::MainScene() {
	
}

void MainScene::Update() {
	// ����n
	if (Input::Key(KEY_INPUT_O) == 1) { // �t�@�C���ǂݍ���
		if (smfio.read("kaeru.mid") == -1) printfDx("�ǂݍ��ݎ��s\n");
		else printfDx("�ǂݍ��ݐ���\n");
	}
	if (Input::Key(KEY_INPUT_S) == 1) { // �t�@�C�������o��
		if (smfio.write("output.mid", 1) == -1) printfDx("�������ݎ��s\n");
		else printfDx("�������ݐ���\n");
	}
	if (Input::Key(KEY_INPUT_A) == 1) { // �Đ�
		PlayMusic("output.mid", DX_PLAYTYPE_BACK);
		printfDx("%s���Đ�\n", "kaeru.mid");
	}
	if (Input::Key(KEY_INPUT_J) == 1) { // �������
		midiEventManager.autoCreate();
	}

	if (Input::Key(KEY_INPUT_D) == 1) StopMusic(); // ��~

	// �f�o�b�O�n
	if (Input::Key(KEY_INPUT_C) == 1) clsDx();
}

void MainScene::Draw(){
	// MIDI�C�x���g��\��
	midiEventManager.draw();

	// ������@��\��
	static const int INFO_X = 400;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,125);
	DrawBox(INFO_X - 20, 80, FMX - 20, 240, WHITE,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatString(INFO_X, 80, WHITE, "������@\nO�L�[�œǍ�\nS�L�[�ŕۑ�\nA�L�[�ōĐ�\nD�L�[�Œ�~\nC�L�[�ŃN���A\nESC�ŏI��");
}