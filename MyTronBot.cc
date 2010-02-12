// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "map.h"
#include "loc.h"
#include "bot.h"
#include <cstdio>

/*==========================================================================*/
/*                           Function Prototypes                            */
/*==========================================================================*/
int main();

/*==========================================================================*/
/*    Function: main                                                        */
/* Description: Entry point for the program                                 */
/*==========================================================================*/
int main()
{
	Map     map;
	AI::Bot bot;
	Loc     loc;

	while (true) {
		map.ReadFromFile(stdin);
		loc = bot.makeMove(map);
		map.makeMove(loc, stdout);
	}

	return 0;
}
