#include "game.h"
#include "view.h"
#include "cntrler.h"

int main()
{
	Game game;
	View view( game );
	Controller ctrl( game, view );

	while ( ctrl.getInput() )
	{
		ctrl.updateGame();
		ctrl.updateView();
	}
	ctrl.SaveScore();
	return 0;
}

