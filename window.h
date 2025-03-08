#ifndef window_h
#define window_h
#include "coord.h"
#include "cursor.h"
#include "header.h"

typedef unsigned char byte;

class Window
{
	int _startY;
	int _startX;
	int _endY;
	int _endX;
	int _height;
	int _width;
	char _attr;
	Cursor&	_cursor;
	int _winoffset;
	int _cursoffset;
	int _crlfShift;
	int _crd2ofs( Coord );
	void _init();
	public:
	Window ();
	Window ( int, int, int, int );
	~Window ();
	void init( int, int, int, int );
	char setattr( char = lgray, char = black, bool = false );
	void blink( bool = true );
	void box( char = ul_corner, char = uh_line,	char = ur_corner,
		 char = lv_line, char = rv_line, char = dl_corner,
		 char = dh_line,	char = dr_corner );
	void clear( char = ' ' );
	void putchar( Coord, char );
	void charblink( Coord );
	void putstr( Coord, char* );
	void hline( Coord, int, char );
	void vline( Coord, int, char );
	Window& operator<<( char );
	Window& operator<< ( char* );
	Window& operator<<( unsigned int );
	Window& operator<<( Coord );

	static int termCol;
	static int termRow;
	static void screenMode();
	static void cursorOn();
};

#endif // window_h
