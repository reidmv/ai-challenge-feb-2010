// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef BOT_H
#define BOT_H

#include "map.h"
#include "loc.h"
#include "astar.h"
#include "floodfill.h"
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

			enum botstate { CHARGE, FILL, SKIRT };

			Loc makeMove(Map &map);

		private:
			botstate       state;
			int            counter;
			std::list<Loc> path;
			std::list<Loc> adjacencies;
			AStar          astar;
			Floodfill      floodfill;

			void charge(Loc &player, Loc &opponent, int oppDist, Map &map);
			void fill(Loc &player, Loc &opponent, int oppDist, Map &map);
			void skirt(Loc &player, Loc &opponent, int oppDist, Map &map);
			void simple(Loc &player, Map &map);
	};
}

#endif
