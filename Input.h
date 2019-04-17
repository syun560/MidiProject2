#pragma once
#include "global.h"

class Input{
	static int mKey[256];
	static char mKeyBuf[256];
	static int mMouseX,mMouseY,mMouseL,mMouseR,mMouseM;
public:
	static void Mouse(int*,int*);
	static int MouseL();
	static int MouseR();
	static int MouseM();
	static int Key(int);
	static void update();
	static void draw();
};