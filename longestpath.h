// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef LONGESTPATH_H
#define LONGESTPATH_H

#include "ai.h"
#include "map.h"
#include "loc.h"
#include <list>
#include <sys/time.h>

namespace AI {

	/*======================================================================*/
	/*       Class: LongestPath                                             */
	/* Description: When given starting coordinates, calculates the longest */
	/*              possible path.                                          */
	/*======================================================================*/
	class LongestPath
	{
		public:
			LongestPath();
			~LongestPath();

			// Returns a list of nodes representing the longest path that could
			// be found starting from the given node in the given map in the 
			// given time (usec).
			std::list<Loc> search(Loc &start,
			                      Map &map,
			                      int maxtime = USEC_MAXTIME);

		private:
			std::list<Loc> path;          // the current path being expored
			std::list<Loc> longest_path;  // the longest path yet found

			struct timeval t_beg;         // The beginning time of a segment
			struct timeval t_end;         // The ending time of a segment   
			struct timeval t_tot;         // The elapsed time of a segment  

			// returns a list of adjacent nodes, given a node and a map
			std::list<Loc> getAdjacencies(Loc &loc, Map &map);

			// returns true if the given node is already in the current path
			bool inPath(Loc &loc);
	};
}

#endif
