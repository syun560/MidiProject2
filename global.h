#pragma once
#include "DxLib.h"
#define _CRT_SECURE_NO_WARNINGS

// �ܓx��
static const char CIRCLE_OF_FIFTH[2][12][5] = {
	"C",  "G",  "D",  "A",    "E",    "B",   "G��",  "D��",  "A��", "E��", "B��", "F",
	"Am", "Em", "Bm", "F��m", "C��m", "G#m", "D��m", "B��m", "Fm",  "Cm",  "Gm",  "Dm"
};

static const int MAJOR_SCALE[8] = {
	0, 0, 2, 4, 5, 7, 9, 11
};

static const char CHORD_NAME[6][12][8] = {
	"C", "D��", "D",  "E��", "E",  "F",  "G��", "G",  "A��", "A",  "B��", "B",
	"Cm", "C#m", "Dm", "D#m", "Em", "Fm", "F#m", "Gm", "G#m", "Am", "B��m", "Bm",
	"C7", "D��7", "D7",  "E��7", "E7",  "F7",  "G��7", "G7",  "A��7", "A7",  "B��7", "B7",
	"Cm7", "C#m7", "Dm7", "D#m7", "Em7", "Fm7", "F#m7", "Gm7", "G#m7", "Am7", "B��m7", "Bm7",
	"CM7", "D��M7", "DM7",  "E��M7", "EM7",  "FM7",  "G��M7", "GM7",  "A��M7", "AM7",  "B��M7", "BM7",
	"CmM7", "C#mM7", "DmM7", "D#mM7", "EmM7", "FmM7", "F#mM7", "GmM7", "G#mM7", "AmM7", "B��mM7", "BmM7",
};

static const char COMMON_CHORD[2][7][8] = {
	"C", "Dm", "Em",  "F", "G", "Am",
	"CM7", "Dm7", "Em7",  "FM7", "G7", "Am7"
};

//�F
static const int WHITE = GetColor(255, 255, 255);
static const int BLACK = GetColor(0, 0, 0);
static const int YELLOW = GetColor(255, 255, 0);
static const int RED = GetColor(255, 0, 0);
static const int BLUE = GetColor(0, 0, 255);
static const int GREEN = GetColor(0, 255, 0);
static const int PURPLE = GetColor(255, 0, 255);
static const int ORANGE = GetColor(255, 69, 0);
static const int CYAN = GetColor(255, 255, 0);

//��ʂ̑傫��
static const int FMX = 800;
static const int FMY = 600;

void DrawEdgeBox(int x1, int y1, int x2, int y2, int FillColor, int LineColor = WHITE);