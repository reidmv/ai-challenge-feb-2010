// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef ASTARNODE_H
#define ASTARNODE_H

#include "loc.h"

namespace AI {

	/*======================================================================*/
	/*       Class: AStarNode                                               */
	/* Description: A basic node type for use in the AStar class            */
	/*======================================================================*/
	class AStarNode : public Loc
	{
		public:
			AStarNode(void);
			~AStarNode(void);

			AStarNode * getPrev(void) const;
			void        set(const Loc &new_loc, AStarNode *new_prev);
			void        set(int x, int z, AStarNode *new_prev);

		private:
			AStarNode *prev;
	};
}

#endif
