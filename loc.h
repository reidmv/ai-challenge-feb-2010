// Google AI Contest
// Reid Vandewiele
// -marut-
// February 9, 2010

#ifndef LOC_H
#define LOC_H

/*==========================================================================*/
/*       Class: Loc                                                         */
/* Description: Holds a set of int cartesian coordinates, row and col.      */
/*==========================================================================*/
class Loc
{
	public:
		Loc(int init_row = 0, int init_col = 0);
		~Loc(void);

		int  getRow(void) const;
		int  getCol(void) const;
		void setRow(int new_row);
		void setCol(int new_col);
		int  distanceFrom(const Loc &comp) const;

		bool operator==(const Loc &comp) const;

	private:
		int row;
		int col;
};

#endif
