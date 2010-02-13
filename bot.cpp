// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "bot.h"
#include "map.h"
#include "loc.h"
#include "astar.h"
#include "longestpath.h"
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
	Loc move;                     // the move to be returned
	player = map.getPlayer();     // set the bot's current location
	opponent = map.getOpponent(); // set the opponent's current location

	// note adjacent locations
	adjacencies = map.getAdjacencies(player);

	// if there's a choice between two areas, choose the larger. period.
	if (chooseSides(map)) {
		move = path.front();
		path.pop_front();
		return move;
	}

	// choose a strategy based on the current state
	switch (state) {
		case CHARGE: charge(map);
			break;
		case FILL: fill(map);
			break;
		case SKIRT: skirt(map);
			break;
		default: simple(map);
			break;
	}

	// if there are no moves in the move list, do SOMETHING
	if (path.empty()) {
		simple(map);
	} 

	move = path.front();
	path.pop_front();

	return move;
}

/*==========================================================================*/
/* chooseSides                                                              */
/*==========================================================================*/
bool Bot::chooseSides(Map &map)
{
	Loc adjacency;
	int cur_floodsize = 0;
	int new_floodsize = 0;
	int floodsize_diffs = -1;
	std::list<Loc>::iterator erase_me;
	std::list<Loc>::iterator i = adjacencies.begin();

	// it's astar. it's fast. we're starting over here.
	path.clear();

	// filter adjacencies to only consider FLOOR
	while (i != adjacencies.end()) {
		if (map.getVal(*i) != Map::FLOOR) {
			erase_me = i;
			i++;
			adjacencies.erase(erase_me);
		} else if ((new_floodsize = map.floodfill(*i)) > cur_floodsize) {
			cur_floodsize = new_floodsize;
			floodsize_diffs++;
			adjacency = *i;
			i++;
		} else {
			i++;
		}
	}

	// if there was a floodsize returned that was larger than the others, take
	// that path.
	if (floodsize_diffs > 0) {
		path.push_front(adjacency);
		return true;
	} else {
		return false;
	}
}

/*==========================================================================*/
/* CHARGE                                                                   */
/*==========================================================================*/
void Bot::charge(Map &map)
{
	int oppDist; // distance metric to opponent

	// default action
	path = astar.search(player, opponent, map);
	if (!path.empty()) {
		oppDist = path.size();
	} else {
		counter = AI::FILL_MOVES;
		state = FILL;
		path = longestpath.search(player, map, 200000);
	}

	// break out of state charge conditions
	if (oppDist < AI::CHARGE_STOP) {
		counter = AI::SKIRT_MOVES;
		state = SKIRT;
		path = longestpath.search(player, map);
	}

	return;
}

/*==========================================================================*/
/* FILL                                                                     */
/*==========================================================================*/
void Bot::fill(Map &map)
{
	// default action
	if (counter > 0 && !path.empty()) {
		counter--;
		return;
	} else {
		path = longestpath.search(player, map);
		counter = AI::FILL_MOVES;
	}

	return;
}

/*==========================================================================*/
/* SKIRT                                                                    */
/*==========================================================================*/
void Bot::skirt(Map &map)
{
	std::list<Loc>::iterator i;	

	// note path to opponent
	path.clear();
	path = astar.search(opponent, player, map);
	
	if (path.size() == 0) {
		path = longestpath.search(player, map);
		return;
	}
	
	// pop off the player location
	path.pop_back();

	for (i = path.begin(); i != path.end(); i++) {
		if (map.getVal(*i) != Map::DANGER && map.getAdjacencies(*i).size() <= 3) {
			return;
		}
	}

	// default action
	if (counter > 0) {
		counter--;
	} else {
		//state = CHARGE;
	}

	if (path.size() == 0) {
		path = longestpath.search(player, map);
		return;
	}

	return;
}

/*==========================================================================*/
/* SIMPLE                                                                   */
/*==========================================================================*/
void Bot::simple(Map &map)
{
	// default action
	std::list<Loc>::iterator i;

	path.clear();

	// look for any possible move
	for (i = adjacencies.begin(); i != adjacencies.end(); i++) {
		if (map.getVal(*i) == Map::FLOOR || map.getVal(*i) == Map::DANGER) {
			path.push_front(*i);
			break;
		}
	}

	// no move? ok. suicide then.
	if (path.empty()) {
		i--;
		path.push_front(*i);
	}

	return;
}
