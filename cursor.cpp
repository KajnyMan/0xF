#include "cursor.h"

Cursor::Cursor( int a, int b, int h, int w )
	: Coord( a, b ), _maxy( h - 1 ), _maxx( w - 1 )
{}
void Cursor::init( int a, int b, int h, int w )
{
	y = a;
	x = b;
	_maxy = h - 1;
	_maxx = w - 1;
}
void Cursor::cr()
{
	x = 0;
}
void Cursor::crlf()
{
	x = 0;
	y < _maxy ? y++ : ( y = 0 );
}
void Cursor::tab()
{
	x += 4;
	if ( x > _maxx )
	{
		x -= _maxx - 1;
		if ( y < _maxy ) y++;
		else y = 0;
	}
}
void Cursor::home()
{
	y = x = 0;
}
Cursor& Cursor::operator=( Coord & crd )
{
	y = crd.y;
	x = crd.x;
	return *this;
}
Cursor& Cursor::operator++()
{
	if ( x < _maxx ) x++;
	else 
	{
		x = 0;
		if ( y < _maxy ) y++;
		else y = 0;
	}
	return *this;
}
