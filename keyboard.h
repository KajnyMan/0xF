#ifndef keyboard_h
#define keyboard_h

#include "header.h"

class Keyboard
{
	int _pkey;
	Direction _pdir;
	public:
	int waitOnKey();
	int getKey(){ return _pkey; }
	Direction getDir();
	int getInput(); 
	bool isArrow();

	static const int quitKey;
	static const int restartKey;
	static const int KEY_DOWN	;
	static const int KEY_UP		;
	static const int KEY_LEFT	;
	static const int KEY_RIGHT	;
};
#endif // keyboard_h
