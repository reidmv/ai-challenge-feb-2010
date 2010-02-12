// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "ai.h"
#include "floodfill.h"
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <list>

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
	Loc       adjacency;
	list<Loc> adjacencies;

	int rowStart = max(loc.getRow() - 1, 0);
	int colStart = max(loc.getCol() - 1, 0);
	int rowStop  = min(loc.getRow() + 1, map.getRows());
	int colStop  = min(loc.getCol() + 1, map.getCols());
	bool diag = (loc.getRow() % 2) - (loc.getCol() % 2);

	adjacencies.clear();

	for (int i = rowStart; i <= rowStop; ++i) {
		for (int j = colStart; j <= colStop; ++j) {

			// skip diagonals
			if (static_cast<bool>((i % 2) - (j % 2)) == diag) {
				continue;
			}

			// note the new location, and add it to result
			adjacency.setRow(i);
			adjacency.setCol(j);
			if (map.getVal(i, j) == Map::FLOOR && !inPath(adjacency)) {
				adjacencies.push_back(adjacency);
			}
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
