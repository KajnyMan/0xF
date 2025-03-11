#include "game.h"
#include "header.h"
#include "coord.h"
#include <stdlib.h> 	// srand() rand()
#include "file.h"

Coord Game::arrsize = Coord( 4, 4 );
Coord Game::numrange = Coord( 1, 15 ); // 2048 = 2^11

Game::Game() :
	_file ( * new File( "score.dat" ) )
{
	_points = _numMovedDigits = 0;
	_highestNum = 1;
	_isMovePossible = true;
	srand ( time( NULL ) );
	freeCells.resize ( arrsize.y * arrsize.x );

	// definicja tablicy 2D pola gry
	_digits = new Digit*[ arrsize.y ];
	for ( int y = 0; y < arrsize.y; y++ )
	{
		_digits[y] = new Digit[ arrsize.x ];
		// a przy okazji init zerami kazdej z linii gry
		for ( int x = 0; x < arrsize.x; x++)
		{
			_digits[y][x] = 0;
			freeCells.push_back( Coord( y, x ) );
		}
	}
	// dwie losowe cyfry w polu gry na poczatek
	newArrayDigit();
	newArrayDigit();
	LoadScore();
}
Game::~Game()
{
	SaveScore();
	for ( int y = 0; y < arrsize.y; y++)
	{
		delete[] _digits[ y ];
	}
	delete [] _digits;
	delete & _file;
}
void Game::restart()
{
	SaveScore();
	if ( _points > _record ) _record = _points;
	if ( _highestNum > _bestHighestNum ) _bestHighestNum = _highestNum;
	freeCells.clear();
	for ( int y = 0; y < arrsize.y; y++ )
	{
		for ( int x = 0; x < arrsize.x; x++ )
		{
			_digits[y][x] = 0;
			freeCells.push_back( Coord( y, x ) );
		}
	}
	_points = _numMovedDigits = 0;
	_highestNum = 1;
	_isMovePossible = true;
	_direction = up;
	newArrayDigit();
	newArrayDigit();
}
void Game::update( Direction d )
{
	_direction = d;
	_move();
	if ( _numMovedDigits )
	{
		newArrayDigit();
		_numMovedDigits = 0;
	}
	if ( !freeCells.size() ) _isMovePossible = checkSameNeighbors();
}
void Game::LoadScore()
{
	if ( F_ERROR == 2 ) _file.create( "score.dat" );
	int rec, besthnum;
	_file >> rec >> besthnum;
	_record = rec;
	_bestHighestNum = besthnum;
	_file.close();
}
void Game::SaveScore()
{
	if ( _points > _record || _highestNum > _bestHighestNum )
	{
		_file.open( "score.dat" );
		if ( _points <= _record ) _file << _record << _highestNum;
		else if ( _highestNum <= _bestHighestNum )
		{
			_file << _points << _bestHighestNum;
		}
		else _file << _points << _highestNum;
		_file.close();
	}
}
void Game::_move()
{
	int it, s;
	for ( int it1 = 0; _cond1( it1 ); it1++ )
	{
		
		_opnum = 0;
		it = _init1();
		while ( _cond2( it ) )
		{
			if ( _cond3( it, it1 ) )
			{
				_f = it;	
				s = _decincval( it );
				while (  _cond4( s )  )
				{
					if ( _moveTile( _f, s, it1, it1 ) ) _opnum++;
					_decinc( _f );
					_decinc( s );
				}
			}
			_incdec( it );
		}
	}	
}
bool Game::_moveTile( int p1, int p2, int p3, int p4 )
{
	if ( _direction == left || _direction == right )
	{
		int p5 = p1;
		int p6 = p2;
		p1 = p3;
		p2 = p4;
		p3 = p5;
		p4 = p6;

	}
	Digit & fd = _digits[p1][p3]; // 1 cyfra w kierunku ruchu
	Digit & sd = _digits[p2][p4]; // 2 cyfra
	if ( !sd )
	{
		sd = fd;
		freeCells.drop( crd2index( Coord( p2, p4 ) ) );
		fd = 0;
		_isOp = false;
		freeCells.push_back( Coord( p1, p3 ) );
	}
	else if ( sd == fd )
	{
		if ( sd < numrange.x ) sd++;
		fd = 0;
		_isOp = true;
		freeCells.push_back( Coord( p1, p3 ) );
		_points += 2 << ( sd - 1 ); // zwieksza o 2 do potegi nowej cyfry
		if ( sd > _highestNum ) _highestNum++;
	}
	else
	{
		_isOp = true;
		return _isOp;
	}
	_numMovedDigits++;
	return _isOp;
}
int Game::_init1()
{
	if ( _direction == up || _direction == left ) return 1; 
	else if ( _direction == down ) return arrsize.y - 2;
	return arrsize.x - 2;
}
int Game::_cond1( int i )
{
	if ( _direction == down || _direction == up ) return i < arrsize.x; 
	return i < arrsize.y;
}
int Game::_cond2( int i )
{
	if ( _direction == down || _direction == right ) return i >= 0; 
	else if ( _direction == up ) return i < arrsize.y; 
	return i < arrsize.x;
}
int Game::_cond3( int p1, int p2 )
{
	if ( _direction == down || _direction == up ) return _digits[p1][p2]; 
	return _digits[p2][p1];
}
int Game::_cond4( int i )
{
	if ( _direction == up || _direction == left ) return i >= _opnum; 
	else if ( _direction == down ) return i < ( arrsize.y - _opnum );
	return i < ( arrsize.x - _opnum );
}
void Game::_incdec( int & itr )
{	
	if ( _direction == up || _direction == left ) itr++;
	else itr--;
}
void Game::_decinc( int & itr )
{	
	if ( _direction == up || _direction == left ) itr--;
	else itr++;
}
int Game::_decincval( int itr )
{	
	if ( _direction == up || _direction == left ) return --itr;
	else return ++itr;
}
int Game::crd2index( Coord c )
{
	for ( int i = 0; i < freeCells.size(); i++ )
	{
		if ( freeCells.at( i ) == c ) return i;
	}
	return -1;
}
Digit Game::drawDigit( Digit a, Digit b )
{
	if ( b >= a ) return rand() % ( b - a + 1 ) + a;
	else return 0;
}
void Game::newArrayDigit()
{
	if ( freeCells.size() )
	{
		Digit index = drawDigit( 0, freeCells.size() - 1 );
		_cell = freeCells[ index ]; // wylosowane miejsce dla cyfry
		Digit& newdigit = _digits[ _cell.y ][ _cell.x ];
		// 80 % 1-ka i 20% 2-ka
		newdigit = numrange.y;
		if ( drawDigit( 0, 9 ) < 2 )
		{
			if ( ++newdigit > _highestNum ) _highestNum++;
		}


		// ten koordynat nie jest juz wolny, a zatem out
		freeCells.drop( index );
	}
}
/* Jesli nie ma wolnych pol funkcja sprawdza czy jest opcja ruchu, tj
	czy jest wsrod sasiednich pol taka sama cyfra. */
bool Game::checkSameNeighbors()
{

	for ( int y = 0; y < arrsize.y; y++ )
	{
		for ( int x = 0; x < arrsize.x; x++ )
		{
			Digit & d = _digits[y][x];	// badana cyfra
			Digit & dr = _digits[y][x +1];	// cyfra po prawej
			Digit & dd = _digits[y + 1][x];	// cyfra pod spodem
			if ( x < arrsize.x - 1 && d == dr ) return true;
			if ( y < arrsize.y - 1 && d == dd ) return true;
		}
	}
	return false;
}
