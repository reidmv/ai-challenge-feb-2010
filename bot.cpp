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
#include <iostream>

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
	bool result;
	Loc  adjacency;
	int  prev_floodsize = 0;
	int  curr_floodsize = 0;
	int  floodsize_diffs = -1;
	std::list<Loc>::iterator erase_me;
	std::list<Loc>::iterator i = adjacencies.begin();

	// it's astar. it's fast. we're starting over here.
	path.clear();

	// filter adjacencies to only consider FLOOR
	while (i != adjacencies.end()) {

		// move along if the adjacency isn't a floor space
		if (map.getVal(*i) != Map::FLOOR) {
			i++;
			continue;
		}

		// select the space with the highest floodfill score
		curr_floodsize = map.floodfill(*i);
		if (curr_floodsize > prev_floodsize) {
			adjacency = *i;
		}
		// keep track of how many different scores there are
		if (curr_floodsize != prev_floodsize) {
			floodsize_diffs++;
		}
		prev_floodsize = curr_floodsize;
		i++;
	}

	// if there was a floodsize returned that was larger than the others, take
	// that path.
	if (floodsize_diffs > 0) {
		path.push_front(adjacency);
		result = true;
	} else {
		result = false;
	}

	return result;
}

/*==========================================================================*/
/* CHARGE                                                                   */
/*==========================================================================*/
void Bot::charge(Map &map)
{
	/////////
	// debug
	#ifdef DEBUG
	std::cerr << "State: CHARGE" << std::endl;
	#endif
	// debug
	/////////

	// if there's a choice between two areas, choose the larger. period.
	if (chooseSides(map)) {
		return;
	}

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
	/////////
	// debug
	#ifdef DEBUG
	std::cerr << "State: FILL" << std::endl;
	#endif
	// debug
	/////////

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

	/////////
	// debug
	#ifdef DEBUG
	std::cerr << "State: SKIRT" << std::endl;
	#endif
	// debug
	/////////

	// if there's a choice between two areas, choose the larger. period.
	if (chooseSides(map)) {
		return;
	}

	// note path to opponent
	path.clear();
	path = astar.search(player, opponent, map);
	
	if (path.size() == 0) {
		state = FILL;
		path = longestpath.search(player, map);
		return;
	}

	// look for chokepoints
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
	/////////
	// debug
	#ifdef DEBUG
	std::cerr << "State: SIMPLE" << std::endl;
	#endif
	// debug
	/////////

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
