#include "Input.h"

int Input::mMouseX = 0;
int Input::mMouseY = 0;
int Input::mMouseL = 0;
int Input::mMouseM = 0;
int Input::mMouseR = 0;
int Input::mKey[256] = {0};
char Input::mKeyBuf[256] = {0};

void Input::update(){
	
	//マウス
	GetMousePoint(&mMouseX,&mMouseY);
	if((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) mMouseL++;
	else mMouseL = 0;
	if((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) mMouseR++;
	else mMouseR = 0;
	if((GetMouseInput() & MOUSE_INPUT_MIDDLE) != 0) mMouseM++;
	else mMouseM = 0;

	//キーボード
	GetHitKeyStateAll(mKeyBuf);
	for(int i=0;i<256;i++){
		if(mKeyBuf[i]==1){
			mKey[i] += 1;
		}
		else if(mKey[i] > 0){
			mKey[i] = 0;
		}
	}	
}

void Input::Mouse(int *x,int *y){
	*x = mMouseX;
	*y = mMouseY;
}

void Input::draw(){
	
}

int Input::Key(int k){
	return mKey[k];
}


int Input::MouseL(){return mMouseL;}
int Input::MouseM(){return mMouseM;}
int Input::MouseR(){return mMouseR;}