#include "header.h"
#include "coord.h"

bool Coord::operator== ( Coord & c )
{
	return ( y == c.y ) && ( x == c.x )? true: false;
}
Coord Coord::operator+ ( int i )
{
	Coord l;
	l.y = y + i;
	l.x = x + i;
	return l;
}
