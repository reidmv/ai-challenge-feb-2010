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
	std::list<Loc>::iterator erase_me;
	std::list<Loc>::iterator i;

	// start the timer
	gettimeofday(&t_beg, NULL);

	// note adjacent traversable locations
	adjacencies = map.getAdjacencies(player);
	i = adjacencies.begin();
	while (i != adjacencies.end()) {
		if ((map.getVal(*i) != Map::FLOOR) && map.getVal(*i) != Map::DANGER) {
			erase_me = i;
			i++;
			adjacencies.erase(erase_me);
		} else {
			i++;
		}
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

		// select the space with the highest floodfill score
		curr_floodsize = map.floodfill(*i);
		if (curr_floodsize > prev_floodsize) {
			adjacency = *i;
		}
		// keep track of how many different scores there are
		if (curr_floodsize != prev_floodsize && map.getVal(*i) != Map::BLOCK) {
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
	int                                   oppDist;
	
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

	// default action
	//path = astar.search(player, opponent, map);
	oppDist = calcBestPath(opponent, map);
	if (oppDist == 0) {
		counter = AI::FILL_MOVES;
		state = FILL;
		path = longestpath.search(player, map, 200000);
		return;
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
/* calcBestPath                                                             */
/*==========================================================================*/
int Bot::calcBestPath(Loc &dest, Map &map) 
{
	std::list<Loc>            temp_path;
	Loc                       first_node;
	std::list<Loc>::iterator  i;
	int                       oppDist;

	temp_path = astar.search(player, opponent, map);
	oppDist   = temp_path.size();

	if (oppDist == 0) {
		return oppDist;
	} else {
		first_node = temp_path.front();
		temp_path.pop_front();
	}
	
	// block all the path points
	for (i = temp_path.begin(); i != temp_path.end(); i++) {
		map.setVal(*i, Map::BLOCK);
	}

	// if one side is bigger, take that path
	if (chooseSides(map)) {
		return oppDist;
	} else {
		temp_path.push_front(first_node);
		path.swap(temp_path);
	}

	return oppDist;
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
		path = longestpath.continueSearch(player, map);
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
		path = longestpath.search(player, map, 200000);
		return;
	}

	// look for chokepoints
	if (hasChokepoint(path, map)) {
		return;
	} else if (counter > 0) {
		counter--;
	} else {
		state = CHARGE;
	}

	path = longestpath.search(player, map);

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

/*==========================================================================*/
/* hasChokepoint                                                            */ 
/*==========================================================================*/
bool Bot::hasChokepoint(std::list<Loc> &chokepath, Map &map)
{
	std::list<Loc>           nodeAdjacencies;
	std::list<Loc>::iterator i;	
	std::list<Loc>::iterator j;	
	int adjacency_count;

	if (path.size() < 2) {
		return false;
	}

	// look for chokepoints on each node in the path to the opponent
	for (i = path.begin(); i != path.end(); i++) {

		// count how many adjacent floor spaces
		nodeAdjacencies = map.getAdjacencies(*i);
		for (j = nodeAdjacencies.begin(); j != nodeAdjacencies.end(); j++) {
			if (map.getVal(*j) == Map::FLOOR) {
				adjacency_count++;
			}
		}

		// if the adjacency count indiactes chokepoint, return true
		if (adjacency_count <= 3) {
			
			/////////
			// debug
			#ifdef DEBUG
			std::cerr << "SKIRT: Chokepoint found" << std::endl;
			#endif
			// debug
			/////////
			return true;
		}
	}

	return false;
}
