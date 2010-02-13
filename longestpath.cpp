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
	list< list<Loc> > adjacencyList;  // stack of adjacency lists
	list<Loc>         emptyAdjacency; // an empty adjacency list for pushing
	int               elapsedtime;    // the time elapsed thus far

	// start the timer
	gettimeofday(&t_beg, NULL);

	// setup; make sure our results are blank
	path.clear();
	longest_path.clear();

	// prime the starting location
	path.push_back(start);
	adjacencyList.push_front(emptyAdjacency);
	adjacencyList.front() = getAdjacencies(start, map);

	do
	{
		// if we can go further, do so!
		if (!adjacencyList.front().empty()) {
			path.push_front(adjacencyList.front().front());
			adjacencyList.front().pop_front();
			//loc = path.front();
			adjacencyList.push_front(emptyAdjacency);
			adjacencyList.front() = getAdjacencies(path.front(), map);
		} else { // otherwise roll back
			// save longest path yet found
			if (path.size() > longest_path.size()) {
				longest_path = path;
			}
			path.pop_front();
			adjacencyList.pop_front();
		}

		// calculate elapsed time
		gettimeofday(&t_end, NULL);
		timersub(&t_end, &t_beg, &t_tot);
		elapsedtime = 1000000 * t_tot.tv_sec + t_tot.tv_usec;

	} while(path.size() > 0 && elapsedtime < maxtime);

	// return the longest path we've found so far
	if (path.size() > longest_path.size()) {
	  path.reverse();
		path.pop_front();
		return path;
	} else {
	  longest_path.reverse();
		longest_path.pop_front();
		return longest_path;
	}
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
