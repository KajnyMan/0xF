#include "window.h"
#include "header.h"

// global - dzieki temu inline asm to widzi/
byte far* CGA = ( byte far* ) 0xB8000000L;
// -- statics --

int Window::termCol = 80;
int Window::termRow = 25;
// ----------------------------
void Window::screenMode()
{
	asm{
		mov	ax,0003h	// bios: txt 80x25 16 kolorow
		int	10h
		mov	ah,01h		// bios: kursor niewidoczny
		mov	cx,2000h
		int	10h
	}
}
void Window::cursorOn()
{
	asm {
		mov	ah,01h		// bios: kursor widoczny
		mov	cx,0607h
		int	10h
	}
}
// ----------   end of statics   ------------------------------
Window::Window() : _cursor( * new Cursor() )
{}
Window::Window( int ys, int xs, int yp, int xp )
	:	_cursor( * new Cursor() )
{
	init( ys, xs, yp, xp );
}
Window::~Window()
{
	delete &_cursor;
}
void Window::init( int ys, int xs, int yp, int xp )
{
	_height		= ys;
	_width		= xs;
	_startY		= yp;
	_startX		= xp;
	_endY = _startY + _height - 1;
	_endX = _startX + _width - 1;
	_winoffset = _cursoffset = _crd2ofs( Coord( 0, 0 ) );
	_crlfShift = ( termCol - _width ) << 1;
	_cursor.init( 0, 0, ys, xs );
	setattr();
}
char Window::setattr( char fgd, char bgd, bool blink )
{
	if ( blink ) bgd |= 0x8;	// wlacza bit 7 dla migania
	else if ( bgd > 0x7 ) bgd &= 0x7;	// wylacza bit 7 dla bgd color
	bgd <<= 4;	// przesowa dolny nible 0 4 bity
	return _attr = bgd | fgd;	// klei oba nible
}
void Window::blink( bool b )
{
	if ( b ) _attr |= 0x80;
	else _attr &= 0x7F;
}
void Window::clear( char chr )
{
	asm{
		mov	si,this
		les	di,CGA
		add	di,[si + ._winoffset]
		mov	ah,[si + ._attr]
		mov	al,chr
		mov	bx,[si + ._height]
		}
	nextline:
	asm{
		mov	cx,[si + ._width]
		rep	stosw
		add	di,[si + ._crlfShift]
		dec	bx
		jnz	nextline
	}
}
void Window::box( char ulc, char uhl, char urc, char lvl,
				 char rvl,	char dlc, char dhl, char drc )
{
	putchar( Coord( 0, 0 ), ulc );
	hline( Coord( 0, 1 ), _width - 2, uhl );
	putchar( Coord( 0, _width - 1 ), urc );
	vline( Coord( 1, 0 ), _height - 2, lvl );
	vline( Coord( 1, _width - 1 ), _height - 2, rvl );
	putchar( Coord( _height - 1, 0 ), dlc );
	hline( Coord( _height - 1, 1 ), _width - 2, dhl );
	putchar( Coord( _height - 1, _width - 1 ), drc );
}
void Window::putchar( Coord crd, char chr )
{
	int ofs = _crd2ofs( crd );
	asm{
			mov	si,this
			les	di,CGA
			add	di,ofs;
			mov	ah,BYTE PTR [ si + ._attr ]
			mov	al,chr
			stosw
	}
}
void Window::charblink( Coord crd )
{
	int ofs = _crd2ofs( crd ) + 1;
	asm {
			les	di,CGA
			add	di,ofs;
			mov	al,BYTE PTR es:[di]
			or	al,0x80
			stosb
	}
}
Window& Window::operator<<( Coord crd )
{
	_cursor = crd;
	_cursoffset = _crd2ofs( _cursor );
	return *this;
}
Window& Window::operator<<( char chr )
{
	switch (chr)
	{
		case '\r': _cursor.cr(); break;
		case '\n': _cursor.crlf();break;
		case '\t': _cursor.tab();break;
		case '\f': _cursor.home();break;
		default:
		{
			asm{
				mov	si,this
				les	di,CGA
				add	di,[si + ._cursoffset]
				mov	ah,BYTE PTR [si + ._attr]
				mov	al,chr
				stosw
			}
		++_cursor;
		}
	}
	_cursoffset = _crd2ofs( _cursor );
	return *this;
}
Window& Window::operator<< ( char* str )
{
	asm{
		mov	bx,this
		les	di,CGA
		add	di,[bx + ._cursoffset]
		mov	si,str
		cld
	}
	again:
	asm{
		lodsb
		cmp	al,00h
		jz	finish
		mov	ah,BYTE PTR [bx + ._attr]
		stosw
	}
	++_cursor;
	if ( ! _cursor.x )
	{
		if ( _cursor.y )
		{
			asm {
				mov	bx,this
				add	di,[bx + ._crlfShift]
			}
		}
		else
		{
			asm {
				mov	bx,this
				mov	di,[bx + ._winoffset]
			}
		}
	}
	else asm mov bx,this
	asm	jmp again
	finish:
	_cursoffset = _crd2ofs( _cursor );
	return *this;
}
Window& Window::operator<<( unsigned int num )
{
	const int maxdignum = 5;
	int div, index = 0;
	unsigned long p10;
	bool nonfront = false;
	char arr[ maxdignum + 1 ] = { 0,0,0,0,0,0 };
	for( int i = maxdignum ; i > 0; i-- )
	{
		p10 = power10( i - 1 );
		div = num / p10;
		if ( div )
		{
			arr[index] = div + 48 ;
			num %= p10;
			nonfront = true;
			index++;
		}
		else if ( nonfront || i == 1 )
		{
			arr[index] = 48;
			index++;
		}
	}

	return *this << arr;
}
void Window::putstr( Coord crd, char* str )
{
	int ofs = _crd2ofs( crd );
	asm{
			mov	si,[str]
			les	di,CGA
			add	di,ofs
			mov	ah,07h
	}
	again:
	asm{
			mov	al,[si]
			cmp	al,00h
			jz	finish
			stosw
			inc	si
			jmp	again
	}
	finish:
}
void Window::hline( Coord crd, int len, char ch )
{
	for( int i = 0; i < len; i++ )
	{
		putchar( crd, ch );
		crd.x++;
	}
}
void Window::vline( Coord crd, int len, char ch )
{
	for( int i = 0; i < len; i++ )
	{
		putchar( crd, ch );
		crd.y++;
	}
}
int Window::_crd2ofs( Coord c )
{
	int ofs = ( ( _startY + c.y ) * termCol + _startX + c.x ) << 1;
	return ofs;
}
