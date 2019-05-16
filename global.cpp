#include "global.h"

void DrawEdgeBox(int x1, int y1, int x2, int y2, int FillColor, int LineColor) {
	DrawFillBox(x1, y1, x2, y2, FillColor);
	DrawLineBox(x1, y1, x2, y2, LineColor);
}