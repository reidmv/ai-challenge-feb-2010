// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "bot.h"
#include "map.h"
#include "loc.h"
#include "astar.h"
#include "floodfill.h"
#include <list>
#include <ctime>
#include <sys/time.h>

using namespace AI;

/*==========================================================================*/
/* Constructor                                                              */
/*==========================================================================*/
Bot::Bot(void)
{
	state = CHARGE;
	counter = 0;
}

/*==========================================================================*/
/* Destructor                                                               */
/*==========================================================================*/
Bot::~Bot(void)
{
}

/*==========================================================================*/
/* makeMove                                                                 */
/*==========================================================================*/
Loc Bot::makeMove(Map &map)
{
	Loc move;
	Loc player = map.getPlayer();
	Loc opponent = map.getOpponent();
	int oppDist = player.distanceFrom(opponent);

	// choose a strategy based on the current state
	switch (state) {
		case CHARGE:
			charge(player, opponent, oppDist, map);
			break;
		case FILL:
			fill(player, opponent, oppDist, map);
			//path = floodfill.search(player, map);
			break;
		case SKIRT:
			skirt(player, opponent, oppDist, map);
			break;
		default:
			break;
	}

	// if there are no moves in the move list, do SOMETHING
	if (path.empty()) {
		path = floodfill.search(player, map, 1);
	} else {
		move = path.front();
		path.pop_front();
	}

	return move;
}

/*==========================================================================*/
/* CHARGE                                                                   */
/*==========================================================================*/
void Bot::charge(Loc &player, Loc &opponent, int oppDist, Map &map)
{
	// default action
	path = astar.search(player, opponent, map);
	if (!path.empty()) {
		oppDist = path.size();
	} else {
		state = FILL;
		path = floodfill.search(player, map, 100000);
	}

	// break out of state charge conditions
	if (oppDist < 7) {
		counter = 3;
		state = SKIRT;
		path = floodfill.search(player, map);
	}

	return;
}

/*==========================================================================*/
/* FILL                                                                     */
/*==========================================================================*/
void Bot::fill(Loc &player, Loc &opponent, int oppDist, Map &map)
{
	// default action
	if (counter > 0 && !path.empty()) {
		counter--;
		return;
	} else {
		path = floodfill.search(player, map);
		counter = 5;
	}

	return;
}

/*==========================================================================*/
/* SKIRT                                                                    */
/*==========================================================================*/
void Bot::skirt(Loc &player, Loc &opponent, int oppDist, Map &map)
{
	// default action
	if (counter > 0) {
		counter--;
	} else {
		state = CHARGE;
	}
	
	path = floodfill.search(player, map);

	return;
}
