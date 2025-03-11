#ifndef cntrler_h
#define cntrler_h

#include "game.h"
#include "view.h"
#include "header.h"
#include "keyboard.h"

class Controller
{
	Game&		_game;
	View&		_view;
	Keyboard&	_kbrd;
	bool		_isMove;
	public:
	Controller( Game&, View& );
	~Controller();
	void updateGame();
	void updateView();
	int getInput();
};
#endif // cntrler_h
