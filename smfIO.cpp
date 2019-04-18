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

	// ヘッダチャンク読み取り
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

	// トラックチャンク読み取り
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

	// ヘッダチャンク書き込み
	if (IsLittleEndian) {
		ConvertEndian(header.size, 4);
		ConvertEndian(header.format, 2);
		ConvertEndian(header.track, 2);
		ConvertEndian(header.division, 2);
	}
	// TODO 読み込み時のファイルデータに依存しているため、実行時に変更可能なものとしたい
	fwrite(&header.type, 4, 1, fp); // MThdで大丈夫？
	fwrite(&header.size, 4, 1, fp); //　固定8だよね？
	fwrite(&header.format, 2, 1, fp); // 4で大丈夫？
	fwrite(&header.track, 2, 1, fp); // 実行時に変更されるかも
	fwrite(&header.division, 2, 1, fp); // ふつうは480
	if (IsLittleEndian) {
		ConvertEndian(header.size, 4);
		ConvertEndian(header.format, 2);
		ConvertEndian(header.track, 2);
		ConvertEndian(header.division, 2);
	}

	// トラックチャンク書き込み
	for (int i = 0; i < header.track.i; ++i) {

		if (i == 1) {
			// サイズ計算して、track[0]に書き込む
			
			track[i].size.i = size;
			track[i].data = data;
		}

		// ここ書き込んだ後にもう一回track[i].sizeが必要になるから2回ConvertEndianが必要なんだなあ
		fwrite(&track[i].type, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}
		fwrite(&track[i].size, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}

		// データ書き込み
		fwrite(track[i].data, 1, track[i].size.i, fp);
	}

	fclose(fp);
	return 0;
}

void smfIO::draw() {
	/*printfDx("ヘッダチャンク情報\n");
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
	}*/
}