// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "astarnode.h"
#include <cstdio>

using namespace AI;

/*==========================================================================*/
/* Constructor                                                              */
/*==========================================================================*/
AStarNode::AStarNode(void)
{
	prev = 0;
}

/*==========================================================================*/
/* Destructor                                                               */
/*==========================================================================*/
AStarNode::~AStarNode(void)
{
}

/*==========================================================================*/
/*    Function: getPrev                                                     */
/* Description: Returns the node's prev pointer                             */
/*==========================================================================*/
AStarNode * AStarNode::getPrev(void) const
{
	return prev;
}

/*==========================================================================*/
/*    Function: set                                                         */
/* Description: Sets the node's attributes                                  */
/*==========================================================================*/
void AStarNode::set(const Loc &new_loc, AStarNode *new_prev)
{
	if (&new_loc == 0) {
		return;
	}

	setRow(new_loc.getRow());
	setCol(new_loc.getCol());
	prev = new_prev;

	return;
}

/*==========================================================================*/
/*    Function: set                                                         */
/* Description: Sets the node's attributes                                  */
/*==========================================================================*/
void AStarNode::set(int x, int z, AStarNode *new_prev)
{
	setRow(x);
	setCol(z);
	prev = new_prev;

	return;
}
