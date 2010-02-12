// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef ASTAR_H
#define ASTAR_H

#include "ai.h"
#include "map.h"
#include "loc.h"
#include "astarnode.h"
#include <list>
#include <sys/time.h>

namespace AI {

	/*======================================================================*/
	/*       Class: AStar                                                   */
	/* Description: When given a set of start and end coordinates and a     */
	/*              map, returns a list of waypoints to follow to reach the */
	/*              end.                                                    */
	/*======================================================================*/
	class AStar
	{
		public:
			AStar(void);
			~AStar(void);

			std::list<Loc> search(Loc &p_start, Loc &p_end, Map &p_map, int margin = USEC_MARGIN);

		private:
			std::list<AStarNode> openList;
			std::list<Loc> closedList;
			AStarNode *curr_node;
			Loc  *start;
			Loc  *end;
			Map  *map;

			struct timeval t_beg;         /* The beginning time of a segment */
			struct timeval t_end;         /* The ending time of a segment    */
			struct timeval t_tot;         /* The elapsed time of a segment   */

			AStarNode* leastCost(std::list<AStarNode> &open_list);
			int  costFrom(AStarNode &loc);
			void addAdjacent(AStarNode &curr);
			bool inClosed(Loc &curr);
			bool inOpen(AStarNode &curr);
	};
}

#endif
