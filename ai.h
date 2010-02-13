// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef AI_H
#define AI_H

namespace AI
{
	const int USEC_MAXTIME = 800000; // max time to calculate astar, floodfill
	const int SKIRT_MOVES  = 1;      // how long to stay in skirt mode
	const int FILL_MOVES   = 5;      // how many floodfilled moves to follow
	const int CHARGE_STOP  = 4;      // distance at which to stop charging
}

#endif
