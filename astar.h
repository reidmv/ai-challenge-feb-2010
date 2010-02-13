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

			// returns a list of nodes starting at p_start and ending at p_end.
			// if the returned list is empty, no path was found.
			std::list<Loc> search(Loc &p_start,
			                      Loc &p_end, 
			                      Map &p_map,
			                      int margin = USEC_MARGIN);

		private:
			std::list<AStarNode> openList; // nodes that can be reached
			std::list<Loc> closedList;     // nodes that have been processed
			AStarNode *curr_node;          // the node currently processing
			Loc  *start;                   // the starting node
			Loc  *end;                     // the destination node
			Map  *map;                     // the map

			struct timeval t_beg;          // The beginning time of a segment
			struct timeval t_end;          // The ending time of a segment   
			struct timeval t_tot;          // The elapsed time of a segment  

			// returns the node of least cost from the openList
			AStarNode* leastCost();
			int  costFrom(AStarNode &loc);
			void addAdjacent(AStarNode &curr);
			bool inClosed(Loc &curr);
	};
}

#endif
