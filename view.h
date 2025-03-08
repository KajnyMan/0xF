#ifndef view_h
#define view_h

#include "header.h"
#include "game.h"
#include "sag.h"
#include "Window.h"

class View
{
	Window & _screen;
	Window & _options;
	Window & _scorelabel;
	Window & _score;
	Window & _board;
	Window & _bestlabel;
	Window & _best;
	Window & _lpanel;
	Window & _rpanel;
	Game &  _game;
	Digit**	_pdigits;
	bool	_record;
	bool	_numrec;
	public:
	View( Game & );
	~View();
	
	void setup();
	void update( bool );
	void coll40();
	void coll80();
	void printBoard();
	void printScore();
	void printPanels();
	void printBest();
	void printGameOver();

	static char hexdigits[16];
	static char	arrows[4];
};

#endif // view_h
