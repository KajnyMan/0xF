#ifndef cntrler_h
#define cntrler_h

#include "game.h"
#include "view.h"
#include "header.h"
#include "file.h"
#include "keyboard.h"

class Controller
{
	Game&		_game;
	View&		_view;
	File&		_file;
	Keyboard&	_kbrd;
	bool		_isMove;
	public:
	Controller( Game&, View& );
	~Controller();
	void LoadScore();
	void SaveScore();
	void updateGame();
	void updateView();
	int getInput();
};
#endif // cntrler_h
