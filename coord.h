#ifndef coord_h
#define coord_h

#include "header.h"

class Coord
{
  public:
	Coord(){}
	Coord( int a, int b ) : y( a ), x( b ) {}
	Digit y;
	Digit x;
	bool operator== ( Coord & c );
	Coord & operator+= ( Coord & );
	Coord operator+ ( int i );
	Coord operator& ( Coord );
	Coord operator+ ( Coord & );
	Coord TurnR90();
};
class Dcoord
{
	public:
	Coord a;
	Coord b;
};

class Icoord
{
    public:
	int i;
	Coord c;
};
#endif // coord_h

