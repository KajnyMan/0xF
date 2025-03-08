#include "cntrler.h"
#include "view.h"
#include "game.h"
#include "header.h"
#include "file.h"

Controller::Controller( Game& g, View& v ): _game(g), _view(v)
	, _file ( * new File( "score.dat" ) )
	, _kbrd ( * new Keyboard() )
{
	LoadScore();
	_view.setup();
	_view.update( true );
}
Controller::~Controller()
{
	delete & _kbrd;
	delete & _file;
}
void Controller::LoadScore()
{
	if ( F_ERROR == 2 ) _file.create( "score.dat" );
	int record, bestHighestNum;
	_file >> record >> bestHighestNum;
	_game.setRecord( record );
	_game.setBestHighestNum( bestHighestNum );
	_file.close();
}
void Controller::SaveScore()
{
	unsigned int points = _game.getPoints();
	unsigned int record = _game.getRecord();
	unsigned int hstnum = _game.getHighestNum();
	unsigned int rdhnum = _game.getBestHighestNum();
	if ( points > record || hstnum > rdhnum )
	{
		_file.open( "score.dat" );
		if ( points <= record )	_file << record << hstnum;
		else if ( hstnum <= rdhnum ) _file << points << rdhnum;
		else _file << points << hstnum;
		_file.close();
	}
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
			if ( _game.getPoints() > _game.getRecord() )
			{
				SaveScore();
			}
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
