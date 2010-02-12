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
list<Loc> AStar::search(Loc &p_start, Loc &p_end, Map &p_map, int margin)
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
		least_cost = leastCost(openList);
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
	if (!result.empty()) {
		result.pop_front();
	}

	return result;
}

/*==========================================================================*/
/*    Function: getLeastCost                                                */
/* Description: Returns the node of least cost from the list given          */
/*              (it is expected that the list given is the open list).      */
/*==========================================================================*/
AStarNode* AStar::leastCost(list<AStarNode> &open_list)
{
	list<AStarNode>::iterator i;
	list<AStarNode>::iterator closed;
	list<AStarNode>::iterator minInList;
	int minCost;
	int cost;

	// validate list not empty
	if (open_list.empty()) {
		return 0;
	}

	// set initial minCost to be largest possible int
	minCost = static_cast<unsigned>(-1) >> 1;

	// iterate through the list and return the minimum cost option
	i = open_list.begin();
	minInList = open_list.end();
	while (i != open_list.end()) {
		if (inClosed(*i)) { // if location is closed, del from openList
			closed = i;
			i++;
			open_list.erase(closed);
		} else {            // otherwise, calculate cost
			cost = costFrom(*i);
			if (cost < minCost) {
				minInList = i;
				minCost   = cost;
			}
			i++;
		}
	}

	if (minInList == open_list.end()) {
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
	return end->distanceFrom(curr);
}

/*==========================================================================*/
/*    Function: addAdjacent                                                 */
/* Description: Adds locations adjacent to the given loc to the given       */
/*              open_list                                                   */
/*==========================================================================*/
void AStar::addAdjacent(AStarNode &curr)
{
	AStarNode new_node;
	int rowStart = max(curr.getRow() - 1, 0);
	int colStart = max(curr.getCol() - 1, 0);
	int rowStop  = min(curr.getRow() + 1, map->getRows());
	int colStop  = min(curr.getCol() + 1, map->getCols());
	bool diag = (curr.getRow() % 2) - (curr.getCol() % 2);

	for (int i = rowStart; i <= rowStop; ++i) {
		for (int j = colStart; j <= colStop; ++j) {

			// skip diagonals
			if (static_cast<bool>((i % 2) - (j % 2)) == diag) {
				continue;
			}

			// note the new location, and add it to open_list if not in closed
			new_node.set(i, j, &curr);
			if ( !inClosed(new_node) && 
			     (map->getVal(new_node) != Map::WALL) ) {
				openList.push_back(new_node);
			}
		}
	}

	return;
}

/*==========================================================================*/
/*    Function: inClosed                                                    */
/* Description: Returns true if the given loc is already in the closed list */
/*==========================================================================*/
bool AStar::inClosed(AStarNode &curr)
{
	list<Loc>::iterator i;

	//int debug = closedList.size();

	for (i = closedList.begin(); i != closedList.end(); i++) {
		if (*i == curr) {
			return true;
		}
	}

	return false;
}

/*==========================================================================*/
/*    Function: inOpen                                                      */
/* Description: Returns true if the given loc is already in the open list   */
/*==========================================================================*/
bool AStar::inOpen(AStarNode &curr)
{
	list<AStarNode>::iterator i;

	//int debug = openList.size();

	for (i = openList.begin(); i != openList.end(); i++) {
		if (*i == curr) {
			return true;
		}
	}

	return false;
}
