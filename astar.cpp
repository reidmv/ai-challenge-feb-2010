// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "astar.h"
#include <cmath>
#include <cstdlib>
#include <list>
#include <ctime>
#include <sys/time.h>

using namespace AI;
using namespace std;

/*==========================================================================*/
/* Constructor                                                              */
/*==========================================================================*/
AStar::AStar(void)
{
}

/*==========================================================================*/
/* Destructor                                                               */
/*==========================================================================*/
AStar::~AStar(void)
{
}

/*==========================================================================*/
/*    Function: search                                                      */
/* Description: Returns a list of locations leading from start to end       */
/*==========================================================================*/
list<Loc> AStar::search(Loc &p_start, 
                        Loc &p_end,
                        Map &p_map,
                        bool include_origin,
                        int margin)
{
	list<AStarNode> nodes; // holds nodes constructed during the search
	list<Loc> result;      // the resulting list of locations
	AStarNode new_node;    // a temp
	AStarNode* least_cost; // a temp

	start      = &p_start;  // set the class start to the parameter start
	end        = &p_end;    // set the class end to the parameter end
	map        = &p_map;    // set the class map to the parameter map
	curr_node  = 0;         // no current node
	
	// start the timer
	gettimeofday(&t_beg, NULL);

	// clear the lists
	openList.clear();
	closedList.clear();

	// start by adding the starting location to the open list
	new_node.set(*start, 0);
	openList.push_back(new_node);

	// loop so long as there are nodes available in the open list
	do
	{
		// set the current location
		least_cost = leastCost();
		if (least_cost == 0) {
			break;
		}

		nodes.push_back(*least_cost);  // add it to the list container
		curr_node = &nodes.back();  // and get a pointer into the list

		// if we've reached our destination... yay! build a list to return
		if (*curr_node == *end) {
			while (curr_node != 0) {
				result.push_front(*curr_node);
				curr_node = curr_node->getPrev();
			}
			break;
		}

		// we haven't reached our destination. :(
		closedList.push_front(*curr_node); // location now closed
		addAdjacent(*curr_node); // add adjacent nodes to open_list
		
		// calculate elapsed time
		gettimeofday(&t_end, NULL);
		timersub(&t_end, &t_beg, &t_tot);

	} while (openList.size() != 0 && t_tot.tv_usec < margin);

	// if we managed to find a path, pop the first element (current location)
	if (!result.empty() && !include_origin) {
		result.pop_front();
	}

	return result;
}

/*==========================================================================*/
/*    Function: getLeastCost                                                */
/* Description: Returns the node of least cost from the list given          */
/*              (it is expected that the list given is the open list).      */
/*==========================================================================*/
AStarNode* AStar::leastCost()
{
	list<AStarNode>::iterator i;
	list<AStarNode>::iterator closed;
	list<AStarNode>::iterator minInList;
	int minCost;
	int cost;

	// validate list not empty
	if (openList.empty()) {
		return 0;
	}

	// set initial minCost to be largest possible int
	minCost = static_cast<unsigned>(-1) >> 1;

	// iterate through the list and return the minimum cost option
	i = openList.begin();
	minInList = openList.end();
	while (i != openList.end()) {
		if (inClosed(*i)) { // if location is closed, del from openList
			closed = i;
			i++;
			openList.erase(closed);
		} else {            // otherwise, calculate cost
			cost = costFrom(*i);
			if (cost < minCost) {
				minInList = i;
				minCost   = cost;
			}
			i++;
		}
	}

	if (minInList == openList.end()) {
		return 0;
	}

	return &(*minInList);
}

/*==========================================================================*/
/*    Function: costFrom                                                    */
/* Description: Returns an integer cost to destination from loc             */
/*==========================================================================*/
int AStar::costFrom(AStarNode &curr)
{
	if (map->getVal(curr) == Map::DANGER) {
		return end->distanceFrom(curr) + 100;
	} else {
		return end->distanceFrom(curr);
	}
}

/*==========================================================================*/
/*    Function: addAdjacent                                                 */
/* Description: Adds locations adjacent to the given loc to the openList    */
/*==========================================================================*/
void AStar::addAdjacent(AStarNode &loc)
{
	AStarNode new_node;
	list<Loc>::iterator erase_me;
	list<Loc> adjacencies = map->getAdjacencies(loc);
	list<Loc>::iterator i = adjacencies.begin();

	// remove closed nodes and WALL nodes
	while (i != adjacencies.end()) {
		if (map->getVal(*i) == Map::WALL || inClosed(*i)) {
			erase_me = i;
			i++;
			adjacencies.erase(erase_me);
		} else {
			i++;
		}
	}

	// add remaining adjacencies to openList
	for (i = adjacencies.begin(); i != adjacencies.end(); i++) {
		new_node.set(*i, &loc);
		openList.push_back(new_node);
	}

	return;
}

/*==========================================================================*/
/*    Function: inClosed                                                    */
/* Description: Returns true if the given loc is already in the closed list */
/*==========================================================================*/
bool AStar::inClosed(Loc &curr)
{
	list<Loc>::iterator i;

	for (i = closedList.begin(); i != closedList.end(); i++) {
		if (*i == curr) {
			return true;
		}
	}

	return false;
}
