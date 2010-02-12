// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "ai.h"
#include "floodfill.h"
#include <cmath>
#include <cstdlib>
#include <list>
#include <iostream>

using namespace AI;
using namespace std;

/*==========================================================================*/
/* Constructor                                                              */
/*==========================================================================*/
Floodfill::Floodfill(void)
{
}

/*==========================================================================*/
/* Destructor                                                               */
/*==========================================================================*/
Floodfill::~Floodfill(void)
{
}

/*==========================================================================*/
/* Search                                                                   */
/*==========================================================================*/
list<Loc> Floodfill::search(Loc &start, Map &map, int margin)
{
	list<Loc>         result;
	list< list<Loc> > adjacencyList;
	list<Loc>         emptyAdjacency; 
	Loc               loc;          

	// start the timer
	gettimeofday(&t_beg, NULL);

	path.clear();
	longest_path.clear();

	loc = start;
	path.push_back(loc);
	adjacencyList.push_front(emptyAdjacency);
	adjacencyList.front() = getAdjacencies(loc, map);

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
			loc = path.front();
		}

		// calculate elapsed time
		gettimeofday(&t_end, NULL);
		timersub(&t_end, &t_beg, &t_tot);

	} while(path.size() > 0 && t_tot.tv_usec < margin);

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
/* Adjacencies                                                              */
/*==========================================================================*/
list<Loc> Floodfill::getAdjacencies(Loc &loc, Map &map)
{
	list<Loc>::iterator erase_me;
	list<Loc> adjacencies = map.getAdjacencies(loc);
	list<Loc>::iterator i = adjacencies.begin();

	while (i != adjacencies.end()) {
		if (map.getVal(*i) != Map::FLOOR || inPath(*i)) {
			erase_me = i;
			i++;
			adjacencies.erase(erase_me);
		} else {
			i++;
		}
	}

	return adjacencies;
}

/*==========================================================================*/
/* inPath                                                                   */
/*==========================================================================*/
bool Floodfill::inPath(Loc &loc)
{
	list<Loc>::iterator i;

	for (i = path.begin(); i != path.end(); i++) {
		if (*i == loc) {
			return true;
		}
	}

	return false;
}
