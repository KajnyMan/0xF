#ifndef game_h
#define game_h

#include "header.h"
#include "coord.h"
#include "sag.h"
#include "file.h"

class Game
{
	Digit ** _digits;
	Sag< Coord > freeCells;			// worek z wolnymi polami gry
	unsigned int _points;
	unsigned int _record;
	unsigned int _highestNum;
	unsigned int _bestHighestNum;
	Direction	_direction;
	bool _numMovedDigits;
	bool _isOp;
	bool _isMovePossible;
	int _f;
	int _opnum;
	Coord _cell;
	File& _file;
	int _init1();
	int _cond1( int );
	int _cond2( int );
	int _cond3( int, int );
	int _cond4( int );
	void _incdec( int & );
	void _decinc( int & );
	int _decincval( int );
	void _move();
	bool _moveTile( int, int, int, int );
	public:
	Game();
	~Game();
	void restart();
	void update( Direction );
	void LoadScore();
	void SaveScore();
	int crd2index( Coord );
	Digit** getDigits(){ return _digits; }
	Direction getDirection(){ return _direction; }
	unsigned int getRecord(){ return _record; }
	unsigned int getPoints(){ return _points; }
	unsigned int getHighestNum(){ return _highestNum; }
	unsigned int getBestHighestNum(){ return _bestHighestNum; }
	Coord getCell(){ return _cell; }
	unsigned int getNumFreeCells() { return freeCells.size(); }
	bool getIsMovePossible() { return _isMovePossible; }
	bool checkSameNeighbors();
	Digit drawDigit( Digit, Digit ); 		// losuje liczbe od a do b
	void newArrayDigit();				// losowa cyfra w polu gry
	static Coord arrsize;				// wielkosc pola gry
	static Coord numrange;				// zakres cyfr w grze
};
#endif // game_h
