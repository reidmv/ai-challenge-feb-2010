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

	// see if we're in close quarters with the opponent
	checkCloseQuarters();

	// choose a strategy based on the current state
	switch (state) {
		case CHARGE: charge(map);
			break;
		case FILL: fill(map);
			break;
		case SKIRT: skirt(map);
			break;
		case NECKTONECK: necktoneck(map);
			break;
		case LEADING: leading(map);
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
/* checkCloseQuarters                                                       */
/*==========================================================================*/
void Bot::checkCloseQuarters(void)
{
	std::list<Loc>::iterator i;

	// if leading opponent, keep leading
	if (player.distanceFrom(opponent) == 2 && adjacencies.size() == 3) {
		state = LEADING;
		return;
	}
	
	// if adjacent to opponent, aggressively stay adjacent
	for (i = adjacencies.begin(); i != adjacencies.end(); i++) {
		if (*i == opponent) {
			state = NECKTONECK;
			return;
		}
	}

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
/* NECKTONECK                                                               */
/*==========================================================================*/
void Bot::necktoneck(Map &map)
{
	std::list<Loc>::iterator i;

	// if there's a choice between two areas, choose the larger. period.
	if (chooseSides(map)) {
		return;
	}

	path.clear();
	for (i = adjacencies.begin(); i != adjacencies.end(); i++) {
		if (map.getAdjacencies(*i, Map::DANGER).size() == 1) {
			path.push_front(*i);
			break;
		}
	}

	return;
}

/*==========================================================================*/
/* LEADING                                                                  */
/*==========================================================================*/
void Bot::leading(Map &map)
{
	std::list<Loc>           oppAdjacencies;
	std::list<Loc>           playerAdjacencies;
	std::list<Loc>::iterator i;
	std::list<Loc>::iterator j;
	Loc                      bad;

	// if there's a choice between two areas, choose the larger. period.
	if (chooseSides(map)) {
		return;
	}

	// determine which square leads into the opponent's danger zone
	oppAdjacencies = map.getAdjacencies(opponent, Map::DANGER);
	playerAdjacencies = map.getAdjacencies(player, Map::DANGER);

	// if there's more than one choice, this isn't a leading situation
	if (playerAdjacencies.size() > 1) {
		state = SKIRT;
		skirt(map);
		return;
	}

	// 
	path.clear();
	bad = playerAdjacencies.front();
	for (i = adjacencies.begin(); i != adjacencies.end(); i++) {
		if (bad.getRow() != i->getRow() && bad.getCol() != i->getCol()) {
			path.push_front(*i);	
		}
	}

	return;
}

/*==========================================================================*/
/* CHARGE                                                                   */
/*==========================================================================*/
void Bot::charge(Map &map)
{
	int oppDist;

	// if there's a choice between two areas, choose the larger. period.
	if (chooseSides(map)) {
		return;
	}

	// default action
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
	int chokepoints;

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
	chokepoints = hasChokepoint(path, map);
	if (chokepoints && !(hasChokepoint(path, map) % 2)) {
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
int Bot::hasChokepoint(std::list<Loc> &chokepath, Map &map)
{
	std::list<Loc>::iterator i;	
	std::list<Loc>::iterator j;	
	int                      count = 0;

	if (path.size() < 2) {
		return 0;
	}

	// look for chokepoints on each node in the path to the opponent
	for (i = path.begin(); i != path.end(); i++) {

		// don't consider dangerous squares
		if (map.getVal(*i) == Map::DANGER) {
			break;
		}

		// if the adjacency count indiactes chokepoint, increment count
		if (map.getAdjacencies(*i, Map::FLOOR).size() <= 2) {
			count++;
		}
	}

	return count;
}
