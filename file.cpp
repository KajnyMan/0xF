#include "file.h"

int F_HEADER = 0;
int F_ERROR = 0;

File::File( const char* ch )
{
	open( ch );
};

bool File::isDigit( const char &ch )
{
	if ( ch > 47 && ch < 58 ) return true;
	else return false;
}
bool File::isWhite( const char &ch )
{
	if ( ch < 33 && ch != 0 ) return true;
	else return false;
}
short File::open( const char * p )
{
	asm{
		mov	ah,3Dh
		mov	al,02h
		mov	dx,[ WORD PTR p ]
		int	21h
		jnc	success
		mov	F_ERROR,ax
		mov	F_HEADER,00h
		jmp	end
	}
	success:
	F_ERROR = 0;
	asm mov F_HEADER,ax
	end:
	return F_ERROR;
}
short File::create( const char * f )
{
	asm{
		mov	ah,3Ch
		mov	cx,00h	// atrybuty pliku
		mov	dx,[ WORD PTR f ]
		int	21h
		jnc	success
		mov	F_ERROR,ax
		mov	F_HEADER,00h
		jmp	end
	}
	success:
	F_ERROR = 0;
	asm mov F_HEADER,ax
	end:
	return F_ERROR;
}
short File::close()
{
	int output;
	asm{
		mov	ah,3Eh
		mov	bx,F_HEADER
		int	21h
		mov	output,ax
		jnc	end
	}

	return F_ERROR = output;
	end:
	F_HEADER = 0;
	return output;	// tu bedzie kod bledu
}
char File::get()
{
	char ch;
	char* pch= &ch;
	int zn;
	asm{
		mov	bx,F_HEADER
		mov	cx,1
		mov	ah,3Fh
		mov	dx,pch
		int	21h
		mov	zn,ax
	}
	if( zn ) return ch; else return zn;
}
int File::getline( char* ch, int w, char delim )
{
	char* asdf = ch;
	int noch = 0;
	int count = 0;
	asm{
		mov	bx,F_HEADER
		mov	cx,w
		mov	ah,3Fh
		mov	dx, asdf
		int	21h
		jc	end
		mov	noch,ax
	}
	end:
	while( count < noch && asdf[count] != delim ) count++;
	if ( count < noch ) // if delim
	{
		asdf[count] = 0;
		skip ( count - noch + 1 );  // wskaznik 1 byte po delim
	}

	return count;
}
void File::skip( int s )
{
	asm{
		mov	ax, s
		cwd
		mov	cx,dx
		mov	dx,ax
		mov	ax,4201h
		mov	bx,F_HEADER
		int	21h
	}
}
void File::skipHome()
{
	asm{
		xor	dx,dx
		mov	cx,dx
		mov	ax,4200h
		mov	bx,F_HEADER
		int	21h
	}
}
File& File::read( int& sum )
{
	sum = 0;
	int nod = 0;
	char ch;
	int dgs[6];
	while( isWhite( ch = get() ) ) ;
	for( ; isDigit( ch ); nod++ )
	{
		dgs[nod] = ch & 0x0F; // inna wersja ASCII to Value;
		ch = get();
	}
	for( int i = 0; nod > 0; i++, nod-- )
	{
		sum+= dgs[i] * power( 10, nod - 1 );
	}
	while( isWhite( ch ) ){ ch = get(); }
	skip( -1 ); // powrot do pierwszego non white ch
	return *this;
}
File& File::read( Coord& crd )
{
	read( crd.y );
	read( crd.x );
	return *this;
}
File& File::operator>>( int& dig )
{
	int nob = sizeof( int );
	int * number = new int;
	*number = 0;
	int output;
	asm{
		mov	bx,F_HEADER
		mov	cx,nob
		mov	ah,3Fh
		mov	dx,number
		int	21h
		mov	output,ax
		jc	end
	}
	if ( output == nob ) dig = *number;
	else dig = 0;
	end:
	delete number;
	return *this;
}
File& File::operator<<( int dig )
{
	int nob = sizeof( int );
	int * pnum = new int;
	*pnum = dig;
	int output;
	asm{
		mov	bx,F_HEADER
		mov	cx,nob
		mov	ah,40h
		mov	dx,WORD PTR pnum
		int	21h
		mov	output,ax
		jnc	end
	}
	output = 0;
	end:
	delete pnum;
	return *this;
}
int File::Num2NumOfDigits( unsigned int d )
{
	int pow = 9; // max 4.294.967.295 dla int
	for ( pow; pow >= 0; pow-- )
	{
		if ( d >= power( 10, pow ) ) return pow + 1;
	}
	return 1; // dla d = 0
}


