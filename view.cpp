#include "view.h"
#include "header.h"
#include "game.h"
#include "sag.h"
#include "window.h"

char View::hexdigits[] =  { '0', '1', '2', '3', '4', '5', '6', '7',
			'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
char View::arrows[] = { '^', '>', 'v', '<' };

View::View( Game & game ) :
	_game( game ),
	_screen( * new Window( Window::termRow, Window::termCol, 0, 0 ) ),
	_options( * new Window() ),
	_scorelabel( * new Window() ),
	_score( * new Window() ),
	_board( * new Window() ),
	_bestlabel( * new Window() ),
	_best( * new Window() ),
	_lpanel( * new Window() ),
	_rpanel( * new Window() )
{
	// okno opcji
	int top_height = 3;
	int label_width = 12;
	int top_y = 2;
	int options_x = 3;
	_options.init( top_height, label_width, top_y, options_x );
	// okno etykiet wynikow
	int label_x = Window::termCol / 2 - label_width / 2 - 1;
	_scorelabel.init( top_height, label_width, top_y, label_x );
	// okno wynikow
	int scores_x = label_x + label_width;
	int scores_width = 6;
	_score.init( top_height, scores_width, top_y, scores_x );
	// okno planszy gry
	Coord brdsize = Game::arrsize + 2;
	int pos_y = ( Window::termRow ) / 2 - brdsize.y / 2;
	int pos_x = ( Window::termCol ) / 2 - brdsize.x / 2;
	_board.init( brdsize.y, brdsize.x, pos_y, pos_x );
	// lewy panel
	int panel_width = ( Window::termCol - brdsize.x ) / 2  - 4;
	_lpanel.init( brdsize.y , panel_width, pos_y, 2 );
	// prawy panel
	_rpanel.init( brdsize.y , panel_width, pos_y, pos_x + brdsize.x + 2 );
	// okno etykiet najlepszych wynikow
	pos_y += brdsize.y + 3;
	_bestlabel.init( top_height, label_width, pos_y, label_x );
	// okno najlepszych wynikow
	_best.init( top_height, scores_width, pos_y, scores_x );
	Window::screenMode();		// 80x24 16 kolorow
}
View::~View()
{
	_screen.setattr();
	_screen.clear( ' ' );
	delete & _score;
	delete & _board;
	delete & _screen;
	Window::cursorOn();
}
void View::setup()
{
	_record = _numrec = false;
	_screen.setattr();
	_screen.clear();
	_screen.box();
	_pdigits = _game.getDigits();
	_options.clear();
	_options.setattr( red, blue );
	_options << "q - quit   " << '\n' << '\n' << "r - restart" << '\f';
	_scorelabel.clear();
	_scorelabel	<< "points:" << '\n'
				<< "top number:" << '\n'
				<< "free cells:" << '\f';
	_score.clear();
	_board.box();
	_bestlabel.clear();
	_bestlabel	<< '\t' << "\tBEST\t" << '\n'
		 		<< "points:" << '\n'
				<< "top number:" << '\f';
	_best.clear();
	printBest();
}
void View::update( bool ismove )
{
	printBoard();
	printScore();
	printPanels();
	if ( !ismove ) printGameOver();
}
void View::printBoard()
{
	char ch, color;

	for ( int y = 0; y < Game::arrsize.y; y++ )
	{
		for ( int x = 0; x < Game::arrsize.x; x++ )
		{
			ch = color = _pdigits[y][x];
			// jesli zero to kropka, a jak nie to cyfra hex
			ch ? (ch = hexdigits[ ch ]): (ch = 46);
			_board.setattr( color + 1 );
			_board.putchar( Coord ( y + 1, x + 1 ), ch );
		}
	}
	// nowa cyfra wyrozniona na polu gry - blink ( 7 bit on )
	_board.charblink( _game.getCell() + 1 );
}
void View::printScore()
{
	unsigned int pnt = _game.getPoints();
	unsigned int hnm = _game.getHighestNum();
	unsigned int rec = _game.getRecord();
	unsigned int rhn = _game.getBestHighestNum();
	_score.setattr( yellow );
	if ( !_record && rec && pnt > rec )		// jesli rekord  - miga
	{
		_score.blink( true );
		_record = true;
	}
	_score	<< "     " << '\r';
	_score	<< pnt << '\n';
	_score.setattr( hnm + 1 );
	if ( !_numrec && rhn && hnm > rhn )		// to samo dla rekordowej cyfry
	{
		_score.blink( true );
		_numrec = true;
	}
	_score	<< hnm << '\n';
	_score.setattr( brown );
	_score	<< _game.getNumFreeCells() << ' ' << '\f';
		
}
void View::printPanels()
{

	_lpanel.setattr( _game.getHighestNum() + 1 );
	_lpanel.clear( arrows[ _game.getDirection() ] );
	_rpanel.setattr( _game.getHighestNum() + 1 );
	_rpanel.clear( arrows[ _game.getDirection() ] );
}
void View::printBest()
{
	_best.setattr( green );
	_best	<< '\n' << _game.getRecord() << '\n';
	_best.setattr( red );
	_best	<< _game.getBestHighestNum() << '\f';
}
void View::printGameOver()
{
	_lpanel.clear( '*' );
	_lpanel << '\n' << '\n' << '\t' << '\t' << " G  A  M  E ";
	_rpanel.clear( '*' );
	_rpanel << '\n' << '\n' << '\t' << '\t' << " O  V  E  R ";
}
