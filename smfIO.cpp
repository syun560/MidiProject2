#define _CRT_SECURE_NO_WARNINGS
#include "smfIO.h"
#include "stdio.h"
#include <string.h>

smfIO::smfIO() {
	int check = 1;
	if (*(char *)&check) IsLittleEndian = true;
	else IsLittleEndian = false;
}

void smfIO::ConvertEndian(UIntInByte& input, size_t s) {
	char *tmp;
	tmp = new char[s];
	for (int i = 0; i < s; ++i) {
		tmp[i] = input.c[i];
	}
	for (int i = 1; i <= s; ++i) {
		input.c[i - 1] = tmp[s - i];
	}
	delete[] tmp;
}

int smfIO::read(char* fileName) {
	FILE* fp;
	if ((fp = fopen(fileName, "rb")) == NULL) {
		return -1;
	}

	// �w�b�_�`�����N�ǂݎ��
	fread(&header.type, 4, 1, fp);
	fread(&header.size, 4, 1, fp);
	fread(&header.format, 2, 1, fp);
	fread(&header.track, 2, 1, fp);
	fread(&header.division, 2, 1, fp);

	if (IsLittleEndian) {
		ConvertEndian(header.size, 4);
		ConvertEndian(header.format, 2);
		ConvertEndian(header.track, 2);
		ConvertEndian(header.division, 2);
	}

	// �g���b�N�`�����N�ǂݎ��
	track = new TrackChunk[16];
	for (int i = 0; i < header.track.i; ++i) {
		fread(&track[i].type, 4, 1, fp);
		fread(&track[i].size, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}
		track[i].data = new char[track[i].size.i];
		fread(track[i].data, track[i].size.i, sizeof(char), fp);
	}

	// draw();
	fclose(fp);
	return 0;
}

int smfIO::write(char* fileName, char* data, int size) {
	FILE* fp;
	if ((fp = fopen(fileName, "wb")) == NULL) {
		return -1;
	}

	// �w�b�_�`�����N��������
	if (IsLittleEndian) {
		ConvertEndian(header.size, 4);
		ConvertEndian(header.format, 2);
		ConvertEndian(header.track, 2);
		ConvertEndian(header.division, 2);
	}
	// TODO �ǂݍ��ݎ��̃t�@�C���f�[�^�Ɉˑ����Ă��邽�߁A���s���ɕύX�\�Ȃ��̂Ƃ�����
	fwrite(&header.type, 4, 1, fp); // MThd�ő��v�H
	fwrite(&header.size, 4, 1, fp); //�@�Œ�8����ˁH
	fwrite(&header.format, 2, 1, fp); // 4�ő��v�H
	fwrite(&header.track, 2, 1, fp); // ���s���ɕύX����邩��
	fwrite(&header.division, 2, 1, fp); // �ӂ���480
	if (IsLittleEndian) {
		ConvertEndian(header.size, 4);
		ConvertEndian(header.format, 2);
		ConvertEndian(header.track, 2);
		ConvertEndian(header.division, 2);
	}

	// �g���b�N�`�����N��������
	for (int i = 0; i < header.track.i; ++i) {

		if (i == 1) {
			// �T�C�Y�v�Z���āAtrack[0]�ɏ�������
			
			track[i].size.i = size;
			track[i].data = data;
		}

		// �����������񂾌�ɂ������track[i].size���K�v�ɂȂ邩��2��ConvertEndian���K�v�Ȃ񂾂Ȃ�
		fwrite(&track[i].type, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}
		fwrite(&track[i].size, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}

		// �f�[�^��������
		fwrite(track[i].data, 1, track[i].size.i, fp);
	}

	fclose(fp);
	return 0;
}

void smfIO::draw() {
	/*printfDx("�w�b�_�`�����N���\n");
	printfDx("�^�C�v:%c%c%c%c", header.type.c[0], header.type.c[1], header.type.c[2], header.type.c[3]);
	printfDx(", �T�C�Y:%d", header.size.i);
	printfDx(", �t�H�[�}�b�g:%d", header.format.i);
	printfDx(", �g���b�N��:%d", header.track.i);
	printfDx(", ����\:%d\n", header.division.i);

	for (int i = 0; i < header.track.i; ++i) {
		printfDx("�g���b�N[%02d]", i);
		printfDx(", �^�C�v:%c%c%c%c", track[i].type.c[0], track[i].type.c[1], track[i].type.c[2], track[i].type.c[3]);
		printfDx(", �T�C�Y:%d", track[i].size.i);
		printfDx("\n");
	}*/
}