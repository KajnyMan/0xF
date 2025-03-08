#ifndef cursor_h
#define cursor_h
#include "coord.h"

class Cursor : public Coord
{
	int _maxy;
	int _maxx;
	public:
	Cursor(){}
	Cursor( int, int, int, int );
	void init( int, int ,int, int );
	void cr();
	void crlf();
	void tab();
	void home();
	Cursor& operator++();
	Cursor& operator=( Coord & );
};

#endif // cursor_h
