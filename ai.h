// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef AI_H
#define AI_H

namespace AI
{
	const int USEC_MARGIN = 600000; // max time to calculate astar, floodfill
	const int SKIRT_MOVES = 4;      // how long to stay in skirt mode
	const int FILL_MOVES  = 10;     // how many floodfilled moves to follow
	const int CHARGE_STOP = 6;      // distance at which to stop charging
	const int TICKER_MOD  = 7;      // ticker modded by this for pseudo-random
}

#endif
