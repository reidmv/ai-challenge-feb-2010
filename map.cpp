// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "ai.h"
#include "map.h"
#include "loc.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

/*==========================================================================*/
/* Default Constructor                                                      */
/*==========================================================================*/
Map::Map(void)
{
	rows      = -1;
	cols      = -1;
	map       =  0;
	freespace = 0;
	ticker    = 0;
}

/*==========================================================================*/
/* An actually useful constructor                                           */
/*==========================================================================*/
Map::Map(int newrows, int newcols)
{
	rows      = -1;
	cols      = -1;
	map       =  0;
	freespace =  0;
	setMapSize(newrows, newcols);

	return;
}

/*==========================================================================*/
/* Destructor                                                               */
/*==========================================================================*/
Map::~Map(void)
{
	if (map) {
		for (int i = 0; i < rows; ++i) {
		    delete [] map[i];
		    delete [] grid[i];
		}
		delete [] map;
		delete [] grid;
	}

	map  = 0;
	grid = 0;
}

/*==========================================================================*/
/*    Function: setMapSize                                                  */
/* Description: Sets up the map at a specified size                         */
/*==========================================================================*/
void Map::setMapSize(int newrows, int newcols) 
{
	// Ensure sane sizes have been given
	if (newrows <= 0 || newcols <= 0) {
		return;
	}

	// Deallocate map if necessary
	if (map) {
		for (int i = 0; i < rows; ++i) {
		    delete [] map[i];
				delete [] grid[i];
		}
		delete [] map;
		delete [] grid;
	}

	rows = newrows;
	cols = newcols;

	// Allocate map
	map  = new int*[rows];
	grid = new int*[rows];
	for (int i = 0; i < rows; ++i) {
		map[i]  = new int[cols];
		grid[i] = new int[cols];
	}

	// Initialize map to all FLOOR
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			map[i][j]  = FLOOR;
			grid[i][j] = FLOOR;
		}
	}
	return;
}

/*==========================================================================*/
/*    Function: getRows                                                     */
/* Description: Returns the number of rows in the Map.                      */
/*==========================================================================*/
int Map::getRows()
{
	return rows;
}

/*==========================================================================*/
/*    Function: getCols                                                     */
/* Description: Returns the number of cols in the Map.                      */
/*==========================================================================*/
int Map::getCols()
{
	return cols;
}

/*==========================================================================*/
/*    Function: getVal()                                                    */
/* Description: Returns the map value for a specified row and col.          */
/*==========================================================================*/
int Map::getVal(int getrow, int getcol) const
{
	if (getrow < 0 || getrow >= rows ||
		getcol < 0 || getcol >= cols) {
		return -1;
	}

	return map[getrow][getcol];
}

/*==========================================================================*/
/*    Function: getPlayer()                                                 */
/* Description: Returns the map value for the player                        */
/*==========================================================================*/
Loc Map::getPlayer(void)
{
	return player;
}

/*==========================================================================*/
/*    Function: getOpponent()                                               */
/* Description: Returns the map value for the opponent                      */
/*==========================================================================*/
Loc Map::getOpponent(void)
{
	return opponent;
}

/*==========================================================================*/
/*    Function: getVal()                                                    */
/* Description: Returns the map value for a specified location.             */
/*==========================================================================*/
int Map::getVal(const Loc &loc) const
{
	return getVal(loc.getRow(), loc.getCol());
}

/*==========================================================================*/
/*    Function: setVal()                                                    */
/* Description: Sets the map at a specified coordinates to a given value.   */
/*==========================================================================*/
int Map::setVal(int setrow, int setcol, int value) 
{
	if (setrow < 0 || setrow >= rows ||
	    setcol < 0 || setcol >= cols)
	{
		return -1;
	}

	map[setrow][setcol] = value;

	return 0;
}

/*==========================================================================*/
/*    Function: setVal()                                                    */
/* Description: Sets the map at a specified location to a given value.      */
/*==========================================================================*/
int Map::setVal(Loc &loc, int value) 
{
	return setVal(loc.getRow(), loc.getCol(), value);
}

/*==========================================================================*/
/*    Function: getFreespace()                                              */
/* Description: Returns the number of "free" spaces in the map              */
/*==========================================================================*/
int Map::getFreespace(void) const
{
	return freespace;
}

/*==========================================================================*/
/*    Function: makeMove()                                                  */
/* Description: Prints a specified move to the specified file               */
/*==========================================================================*/
void Map::makeMove(int move, FILE *file_handle) {
  fprintf(file_handle, "%d\n", move);
  fflush(file_handle);
	
	/*
	////////
	// debug
	
	switch (move) {
		case 3: fprintf(stderr, "move: \\/\n");
			break;
		case 1: fprintf(stderr, "move: /\\\n");
			break;
		case 2: fprintf(stderr, "move: >>\n");
			break;
		case 4: fprintf(stderr, "move: <<\n");
			break;
	}
  fflush(stderr);


	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			fprintf(stderr, "%d, ", map[i][j]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
	fflush(stderr);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			fprintf(stderr, "%d, ", grid[i][j]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
	fflush(stderr);

	// debug
	////////
	*/
}

/*==========================================================================*/
/*    Function: makeMove()                                                  */
/* Description: Prints a specified move to the specified file               */
/*==========================================================================*/
void Map::makeMove(Loc &move, FILE *file_handle)
{
	int intmove;

	if (move.getRow() > player.getRow()) {
		intmove = 2; // east
	} else if (move.getRow() < player.getRow()) {
		intmove = 4; // west
	} else if (move.getCol() > player.getCol()) {
		intmove = 3; // south
	} else {
		intmove = 1; // north
	}

	makeMove(intmove, file_handle);
}

/*==========================================================================*/
/*    Function: ReadFromFile()                                              */
/* Description: Reads the map state from a file. Hacked in from example     */
/*              code cause even though it's ugly, it's not worth redoing.   */
/*==========================================================================*/
void Map::ReadFromFile(FILE *file_handle) {
  int x, y, c;
	
	int map_width  = rows;
	int map_height = cols;
	int player_one_x;
	int player_one_y;
	int player_two_x;
	int player_two_y;
	
	freespace = 0;

  int num_items = fscanf(file_handle, "%d %d\n", &map_width, &map_height);
  if (feof(file_handle) || num_items < 2) {
    exit(0); // End of stream means end of game. Just exit.
  }

	if (map == 0 || map_width != rows || map_height != cols) {
		setMapSize(map_width, map_height);
	}

  x = 0;
  y = 0;
  while (y < map_height && (c = fgetc(file_handle)) != EOF) {
    switch (c) {
    case '\r':
      break;
    case '\n':
      if (x != map_width) {
	fprintf(stderr, "x != width in Board_ReadFromStream\n");
	return;
      }
      ++y;
      x = 0;
      break;
    case '#':
      if (x >= map_width) {
	fprintf(stderr, "x >= width in Board_ReadFromStream\n");
	return;
      }
      map[x][y] = WALL;
      ++x;
      break;
    case ' ':
      if (x >= map_width) {
	fprintf(stderr, "x >= width in Board_ReadFromStream\n");
	return;
      }
      map[x][y] = FLOOR;
      ++freespace;
      ++x;
      break;
    case '1':
      if (x >= map_width) {
	fprintf(stderr, "x >= width in Board_ReadFromStream\n");
	return;
      }
      map[x][y] = ENTITY;
      player_one_x = x;
      player_one_y = y;
      ++x;
      break;
    case '2':
      if (x >= map_width) {
	fprintf(stderr, "x >= width in Board_ReadFromStream\n");
	return;
      }
      map[x][y] = ENTITY;
      player_two_x = x;
      player_two_y = y;
      ++x;
      break;
    default:
      fprintf(stderr, "unexpected character %d in Board_ReadFromStream", c);
      return;
    }
  }

	player.setRow(player_one_x);
	player.setCol(player_one_y);

	opponent.setRow(player_two_x);
	opponent.setCol(player_two_y);

	//markDanger();
}

/*==========================================================================*/
/* markDanger                                                               */
/*==========================================================================*/
void Map::markDanger(void)
{
	list<Loc> adjacencies = getAdjacencies(opponent);
	list<Loc>::iterator i = adjacencies.begin();

	// mark squares next to opponent as DANGER
	for (i = adjacencies.begin(); i != adjacencies.end(); i++) {
		if (getVal(*i) == FLOOR) {
			map[i->getRow()][i->getCol()] = DANGER;
		}
	}

	return;
}

/*==========================================================================*/
/*    Function: getAdjacencies                                              */
/* Description: Returns a list of locations adjacent to the given loc. Does */
/*              not return walls or entities.                               */
/*==========================================================================*/
list<Loc> Map::getAdjacencies(Loc &loc)
{
	Loc       adjacency;
	list<Loc> adjacencies;

	int rowStart = max(loc.getRow() - 1, 0);
	int colStart = max(loc.getCol() - 1, 0);
	int rowStop  = min(loc.getRow() + 1, rows);
	int colStop  = min(loc.getCol() + 1, cols);
	bool diag = (loc.getRow() % 2) - (loc.getCol() % 2);

	for (int i = rowStart; i <= rowStop; ++i) {
		for (int j = colStart; j <= colStop; ++j) {

			// skip diagonals
			if (static_cast<bool>((i % 2) - (j % 2)) == diag) {
				continue;
			}

			// note the new location, and add it to the list of adjacencies
			adjacency.setRow(i).setCol(j);
			adjacencies.push_back(adjacency);
		}
	}

	return adjacencies;
}

/*==========================================================================*/
/*    Function: floodfillScore                                              */
/* Description: Returns a floodfill metric for the specified location.      */
/*==========================================================================*/
int  Map::floodfillScore(Loc &loc)
{
	// reset the grid
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			grid[i][j] = map[i][j];
		}
	}

	// recursively floodfill
	return floodfillRecurse(loc.getRow(), loc.getCol());
}

/*==========================================================================*/
/*    Function: floodfillRecurse                                            */
/* Description: Recursive floodfill                                         */
/*==========================================================================*/
int  Map::floodfillRecurse(int row, int col)
{
	int count = 1;
	if (row >= 0    ||
	    row <  rows || 
	    col >= 0    ||
	    col <  cols || 
			map[row][col] != FLOOR)
	{
		return 0;
	}

	grid[row][col] = WALL;

	count += floodfillRecurse(row, col + 1);
	count += floodfillRecurse(row, col - 1);
	count += floodfillRecurse(row + 1, col);
	count += floodfillRecurse(row - 1, col);

	return count;
}
