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
			                      bool include_origin = false,
			                      int margin = USEC_MAXTIME);

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

			// returns the cost to the destination from the specificed loc
			int  costFrom(AStarNode &loc);

			// adds nodes adjacent to curr to the openList
			void addAdjacent(AStarNode &curr);

			// returns true if the curr Loc is in the closedList
			bool inClosed(Loc &curr);
	};
}

#endif
