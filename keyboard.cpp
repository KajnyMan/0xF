#include "Keyboard.h"
#include "header.h"

const int Keyboard::KEY_DOWN   = 0x5000;
const int Keyboard::KEY_UP     = 0x4800;
const int Keyboard::KEY_LEFT   = 0x4B00;
const int Keyboard::KEY_RIGHT  = 0x4D00;
const int Keyboard::quitKey    = 0x1071;
const int Keyboard::restartKey = 0x1372;

int Keyboard::waitOnKey(){
	int key;
	asm{
		xor	ax,ax
		mov	ah,00h
		int	16h
		mov	key,ax
	}
	return key;
}
int Keyboard::getInput()
{
	_pkey = waitOnKey();
	return _pkey;
}
bool Keyboard::isArrow()
{
	return ( _pkey == KEY_DOWN || _pkey == KEY_RIGHT ||
		_pkey == KEY_UP || _pkey == KEY_LEFT )? true: false;
}
Direction Keyboard::getDir()
{
	if ( isArrow() )
	{
		switch ( _pkey )
		{
			case KEY_DOWN:	_pdir = down;  break;
			case KEY_UP:	_pdir = up;    break;
			case KEY_LEFT:	_pdir = left;  break;
			case KEY_RIGHT:	_pdir = right; break;
		}
	}
	else _pdir = up;
	return _pdir;
}

