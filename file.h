#ifndef File_h
#define File_h

#include "coord.h"

/*	Klasa do obslugi plikow poprzez przerwania MS DOS.
	 Ma zachowywac sie podobnie do fstream.	*/
class File
{
	short _error;
	public:
	File(){}
	File( const char* ch );
	short open( const char * );
	short create( const char * );
	short close();
	short geterror(){ return _error; }
	void skip( int = 1 );
	void skipHome();
	char get();
	int getline( char*, int = 80, char = '\n' );
//	inline int power( int, int );
	int Num2NumOfDigits( unsigned int d );
	File& read( int& );
	File& read( Coord&);
	// operators
	File& operator>>( int& );
	File& operator>>( Coord&);
	File& operator<<( int );
	// bools
	bool isDigit( const char &ch );
	bool isWhite( const char &ch );
};
#endif	// File_h
