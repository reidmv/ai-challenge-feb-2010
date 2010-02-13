// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef MAP_H
#define MAP_H

#include "loc.h"
#include <list>
#include <cstdio>

/*==========================================================================*/
/*       Class: Map                                                         */
/* Description: Holds a 2-dimensional map (matrix) of integer values.       */
/*==========================================================================*/
class Map
{
	public:
		Map(void);
		Map(int newrows, int newcols);
		~Map(void); 

		int  getRows(void);
		int  getCols(void);
		int  getVal(int getrow, int getcol) const;
		int  getVal(const Loc &loc) const;
		int  setVal(int row, int col, int value);
		int  setVal(Loc &loc, int value);
		void setMapSize(int newrows, int newcols); 
		Loc  getPlayer(void);
		Loc  getOpponent(void);
		int  getFreespace(void) const;
		std::list<Loc> getAdjacencies(Loc &loc);
		int  floodfillScore(Loc &loc);

		void makeMove(int move, FILE *file_handle);
		void makeMove(Loc &move, FILE *file_handle);
		void ReadFromFile(FILE *file_handle);

		enum Val { FLOOR, WALL, ENTITY, DANGER };

	private:
		int  **map;
		int  **grid;
		int  rows;
		int  cols;
		int  freespace;
		int  ticker;
		Loc  player;
		Loc  opponent;

		void markDanger(void);
		int  floodfillRecurse(int row, int col);
};

#endif
