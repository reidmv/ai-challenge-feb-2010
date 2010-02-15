// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef BOT_H
#define BOT_H

#include "map.h"
#include "loc.h"
#include "astar.h"
#include "longestpath.h"
#include <list>

namespace AI {

	/*======================================================================*/
	/*       Class: Bot                                                     */
	/* Description: The state machine that is a Tron bot                    */
	/*======================================================================*/
	class Bot
	{
		public:
			Bot(void);
			~Bot(void);

			// The possible states a bot can be in
			enum State { CHARGE, FILL, SKIRT };

			// Given a map, the bot will return the move it would make
			Loc makeMove(Map &map);

		private:
			int            counter;     // a ticker used to trigger state changes
			State          state;       // the current state of the bot
			std::list<Loc> path;        // the path the bot is following
			std::list<Loc> adjacencies; // a list of nodes currently adjacent
			AStar          astar;       // an AStar object for pathfinding
			LongestPath    longestpath; // a LongestPath object for filling
			Loc            player;      // the bot's current location
			Loc            opponent;    // the opponent's current location

			struct timeval t_beg;       // The beginning time of a segment
			struct timeval t_end;       // The ending time of a segment   
			struct timeval t_tot;       // The elapsed time of a segment  

			// calculates the bot's move when in state CHARGE
			void charge(Map &map);

			// calculates the bot's move when in state FILL
			void fill(Map &map);
			
			// calculates the bot's move when in state SKIRT
			void skirt(Map &map);

			// a backup routine to calculate a move when all else fails
			void simple(Map &map);

			// choose a best path to the given location
			int calcBestPath(Loc &loc, Map &map);

			// performs floodfill on all adjacencies to choose a room to favor
			bool chooseSides(Map &map);

			// returns true if there exists a chokepoint in given path
			bool hasChokepoint(std::list<Loc> &chokepath, Map &map);
	};
}

#endif
