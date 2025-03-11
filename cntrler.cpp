#include "cntrler.h"
#include "view.h"
#include "game.h"
#include "header.h"

Controller::Controller( Game& g, View& v ): _game(g), _view(v)
//	, _file ( * new File( "score.dat" ) )
	, _kbrd ( * new Keyboard() )
{
	_view.setup();
	_view.update( true );
}
Controller::~Controller()
{
	delete & _kbrd;
}
void Controller::updateGame()
{
	_game.update( _kbrd.getDir() );
}
void Controller::updateView()
{
	_isMove = _game.getIsMovePossible();
	_view.update( _isMove );
}
int Controller::getInput()
{
	int ky;
	do
	{
		ky = _kbrd.getInput();
		if ( ky == Keyboard::quitKey ) return 0;
		if ( ky == Keyboard::restartKey )
		{
			_game.restart();
			_view.setup();
			_view.update( true );
			if ( getInput() ) return 1;
			else return 0;
		}
	}
	while ( ! _kbrd.isArrow() );
	return 1;
}
