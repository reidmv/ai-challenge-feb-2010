// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#include "loc.h"
#include <cmath>
#include <cstdlib>

/*==========================================================================*/
/* Default Constructor                                                      */
/*==========================================================================*/
Loc::Loc(int init_row, int init_col)
{
	row = init_row;
	col = init_col;
}

/*==========================================================================*/
/* Destructor                                                               */
/*==========================================================================*/
Loc::~Loc(void)
{
}

/*==========================================================================*/
/*    Function: getRow                                                      */
/* Description: Returns the row                                             */
/*==========================================================================*/
int Loc::getRow(void) const
{
	return row;
}

/*==========================================================================*/
/*    Function: getCol                                                      */
/* Description: Returns the col                                             */
/*==========================================================================*/
int Loc::getCol(void) const
{
	return col;
}

/*==========================================================================*/
/*    Function: setRow                                                      */
/* Description: Sets the row                                                */
/*==========================================================================*/
void Loc::setRow(int new_row)
{
	row = new_row;
	return;
}

/*==========================================================================*/
/*    Function: setCol                                                      */
/* Description: Sets the col                                                */
/*==========================================================================*/
void Loc::setCol(int new_col)
{
	col = new_col;
	return;
}

/*==========================================================================*/
/*    Function: distanceFrom                                                */
/* Description: Approximates a distance metric from this to comp            */
/*==========================================================================*/
int Loc::distanceFrom(const Loc &comp) const
{
	int x = this->row - comp.row;
	int y = this->col - comp.col;

	return ((x * x) + (y * y));
}

/*==========================================================================*/
/*    Function: operator==                                                  */
/* Description: Overloads the == operator for comparing to another Loc      */
/*==========================================================================*/
bool Loc::operator==(const Loc &comp) const
{
	return (this->row == comp.row && this->col == comp.col);
}

/*==========================================================================*/
/*    Function: operator!=                                                  */
/* Description: Overloads the != operator for comparing to another Loc      */
/*==========================================================================*/
bool Loc::operator!=(const Loc &comp) const
{
	return (this->row != comp.row || this->col != comp.col);
}
