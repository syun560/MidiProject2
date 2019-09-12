#define _CRT_SECURE_NO_WARNINGS
#include "smfIO.h"
#include "stdio.h"
#include <string.h>

smfIO::smfIO() {
	int check = 1;
	if (*(char *)&check) IsLittleEndian = true;
	else IsLittleEndian = false;

	header.type.c[0] = 'M'; header.type.c[1] = 'T'; header.type.c[2] = 'h'; header.type.c[3] = 'd';
	header.size.i = 6;
	header.format.i = 1;
	header.track.i = 18; // ここ注意
	header.division.i = 480;
}

void smfIO::ConvertEndian(char* input, size_t s) {
	char *tmp;
	tmp = new char[s];
	for (int i = 0; i < s; ++i) {
		tmp[i] = input[i];
	}
	for (int i = 1; i <= s; ++i) {
		input[i - 1] = tmp[s - i];
	}
	delete[] tmp;
}

char* smfIO::getTrackData(int t) {
	return track[t].data;
}

int smfIO::getTrackSize(int t) {
	return track[t].size.i;
}

int smfIO::read(char* fileName) {
	FILE* fp;
	if ((fp = fopen(fileName, "rb")) == NULL) {
		return -1;
	}

	// ヘッダチャンク読み取り
	fread(&header.type, 4, 1, fp);
	fread(&header.size, 4, 1, fp);
	fread(&header.format, 2, 1, fp);
	fread(&header.track, 2, 1, fp);
	fread(&header.division, 2, 1, fp);

	if (IsLittleEndian) {
		ConvertEndian(header.size.c, 4);
		ConvertEndian(header.format.c, 2);
		ConvertEndian(header.track.c, 2);
		ConvertEndian(header.division.c, 2);
	}

	// トラックチャンク読み取り
	if (track != NULL) delete[] track;
	track = new TrackChunk[header.track.i];
	for (int i = 0; i < header.track.i; ++i) {
		fread(&track[i].type, 4, 1, fp);
		fread(&track[i].size, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size.c, 4);
		}
		if (track[i].data != NULL) delete[] track[i].data;
		track[i].data = new char[track[i].size.i];
		fread(track[i].data, track[i].size.i, sizeof(char), fp);
	}

	
	fclose(fp);
	return 0;
}

int smfIO::getTrackNum() {
	return header.track.i;
}

void smfIO::kakikomi(FILE* fp, UIntInByte& input, int size, bool convertFlag) {
	if (IsLittleEndian && convertFlag) {
		for (int i = size - 1; i >= 0; --i) {
			fputc(input.c[i], fp);
		}
	}
	else fwrite(&input, size, 1, fp);
}

int smfIO::write(char* fileName, char* data, int size) {
	FILE* fp;
	if ((fp = fopen(fileName, "wb")) == NULL) {
		return -1;
	}

	// ヘッダチャンク書き込み
	kakikomi(fp, header.type, 4, false);
	kakikomi(fp, header.size, 4);
	kakikomi(fp, header.format, 2);
	kakikomi(fp, header.track, 2);
	kakikomi(fp, header.division, 2);

	// トラックチャンク書き込み
	for (int i = 0; i < header.track.i; ++i) {

		if (i == 3) {
			// 引数からサイズとデータのアドレスを受け取る
			
			track[i].size.i = size;
			//track[i].data = new char[size];
			track[i].data = data;
		}

		// ここ書き込んだ後にもう一回track[i].sizeが必要になるから2回ConvertEndianが必要なんだなあ
		fwrite(&track[i].type, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size.c, 4);
		}
		fwrite(&track[i].size, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size.c, 4);
		}

		// データ書き込み
		fwrite(track[i].data, 1, track[i].size.i, fp);
	}

	fclose(fp);
	return 0;
}

void smfIO::draw() {
	printfDx("ヘッダチャンク情報\n");
	printfDx("タイプ:%c%c%c%c", header.type.c[0], header.type.c[1], header.type.c[2], header.type.c[3]);
	printfDx(", サイズ:%d", header.size.i);
	printfDx(", フォーマット:%d", header.format.i);
	printfDx(", トラック数:%d", header.track.i);
	printfDx(", 分解能:%d\n", header.division.i);

	for (int i = 0; i < header.track.i; ++i) {
		printfDx("トラック[%02d]", i);
		printfDx(", タイプ:%c%c%c%c", track[i].type.c[0], track[i].type.c[1], track[i].type.c[2], track[i].type.c[3]);
		printfDx(", サイズ:%d", track[i].size.i);
		printfDx("\n");
	}
}