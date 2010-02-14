// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "ai.h"
#include "longestpath.h"
#include <cmath>
#include <cstdlib>
#include <list>

using namespace AI;
using namespace std;

/*==========================================================================*/
/* Constructor                                                              */
/*==========================================================================*/
LongestPath::LongestPath(void)
{
}

/*==========================================================================*/
/* Destructor                                                               */
/*==========================================================================*/
LongestPath::~LongestPath(void)
{
}

/*==========================================================================*/
/*    Function: search                                                      */
/* Description: Returns a list of nodes representing the longest path found */
/*              starting at the given node. The search will terminate when  */
/*              either maxtime has progressed or all possible paths have    */
/*              been explored.                                              */
/*==========================================================================*/
list<Loc> LongestPath::search(Loc &start, Map &map, int maxtime)
{
	return newSearch(start, map, maxtime);
}

/*==========================================================================*/
/*    Function: newSearch                                                   */
/* Description: Begins a new search with a clean set of lists               */
/*==========================================================================*/
list<Loc> & LongestPath::newSearch(Loc &start, Map &map, int maxtime)
{
	// setup; make sure our lists are blank
	path.clear();
	longest_path.clear();
	adjacencyList.clear();
	emptyAdjacency.clear();

	// prime the starting location
	path.push_back(start);
	adjacencyList.push_back(emptyAdjacency);
	adjacencyList.back() = getAdjacencies(start, map);

	// let 'er rip!
	return generatePath(start, map, maxtime);
}

/*==========================================================================*/
/*    Function: continueSearch                                              */
/* Description: Assumes an already in-progress search has been started and  */
/*              continues from where the last search left off.              */
/*==========================================================================*/
list<Loc> LongestPath::continueSearch(Loc &start, Map &map, int maxtime)
{
	list<Loc>::iterator i;

	// move the beginning of the lists to match the new origin point
	for (i = path.begin(); i != path.end() && *i != start; i++) {
		path.pop_front();
		longest_path.pop_front();
		adjacencyList.pop_front();
	}

	// if the path is now empty, the search is not continueable. Start anew.
	if (path.empty()) {
		return newSearch(start, map, maxtime);
	} else {
		return generatePath(start, map, maxtime);
	}
}

/*==========================================================================*/
/*    Function: generatePath                                                */
/* Description: The internal engine used by search() and continueSearch()   */
/*==========================================================================*/
list<Loc> & LongestPath::generatePath(Loc &start, Map &map, int maxtime)
{
	int elapsedtime; // the time elapsed thus far

	// start the timer
	gettimeofday(&t_beg, NULL);

	do
	{
		// if we can go further, do so!
		if (!adjacencyList.back().empty()) {
			path.push_back(adjacencyList.back().back());
			adjacencyList.back().pop_back();
			//loc = path.back();
			adjacencyList.push_back(emptyAdjacency);
			adjacencyList.back() = getAdjacencies(path.back(), map);
		} else { // otherwise roll back
			// save longest path yet found
			if (path.size() > longest_path.size()) {
				longest_path = path;
			}
			path.pop_back();
			adjacencyList.pop_back();
		}

		// calculate elapsed time
		gettimeofday(&t_end, NULL);
		timersub(&t_end, &t_beg, &t_tot);
		elapsedtime = 1000000 * t_tot.tv_sec + t_tot.tv_usec;

	} while(path.size() > 0 && elapsedtime < maxtime);

	// return the longest path we've found so far
	if (path.size() > longest_path.size()) {
		longest_path = path;
	}

	// pop off the front of the stack, which is the player's current location
	if (!longest_path.empty()) {
		path.pop_front();
		longest_path.pop_front();
		adjacencyList.pop_front();
	}

	return longest_path;
}

/*==========================================================================*/
/*    Function: getAdjacencies                                              */
/* Description: Returns a list of nodes adjacent to the given location on   */
/*              the given map.                                              */
/*==========================================================================*/
list<Loc> LongestPath::getAdjacencies(Loc &loc, Map &map)
{
	list<Loc>::iterator erase_me;
	list<Loc> adjacencies = map.getAdjacencies(loc);
	list<Loc>::iterator i = adjacencies.begin();

	// clear adjacent nodes not of type FLOOR
	while (i != adjacencies.end()) {
		if ((map.getVal(*i) != Map::FLOOR) || inPath(*i)) {
			erase_me = i;
			i++;
			adjacencies.erase(erase_me);
		} else {
			i++;
		}
	}

	// return adjacent nodes
	return adjacencies;
}

/*==========================================================================*/
/*    Function: inPath                                                      */
/* Description: Returns true if the given node is already in the current    */
/*              path.                                                       */
/*==========================================================================*/
bool LongestPath::inPath(Loc &loc)
{
	list<Loc>::iterator i;

	for (i = path.begin(); i != path.end(); i++) {
		if (*i == loc) {
			return true;
		}
	}

	return false;
}
