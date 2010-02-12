// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef FLOODFILL_H
#define FLOODFILL_H

#include "ai.h"
#include "map.h"
#include "loc.h"
#include <list>
#include <sys/time.h>

namespace AI {

	/*======================================================================*/
	/*       Class: Floodfill                                               */
	/* Description: When given starting coordinates, calculates the longest */
	/*              possible path.                                          */
	/*======================================================================*/
	class Floodfill
	{
		public:
			Floodfill();
			~Floodfill();

			std::list<Loc> search(Loc &start, Map &map, int margin = USEC_MARGIN);

		private:
			std::list<Loc> path;
			std::list<Loc> longest_path;

			struct timeval t_beg;         /* The beginning time of a segment */
			struct timeval t_end;         /* The ending time of a segment    */
			struct timeval t_tot;         /* The elapsed time of a segment   */

			std::list<Loc> getAdjacencies(Loc &loc, Map &map);
			bool           inPath(Loc &loc);
	};
}

#endif
