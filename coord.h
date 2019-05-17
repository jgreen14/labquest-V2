#ifndef COORD_H
#define COORD_H

class Coord
{
	private:
		int y;
		int x;

	public:
		Coord();
		Coord(int, int);
	
		int getY();
		void setY(int);
		int getX();
		void setX(int);
		void setYX(int, int);
        bool equal(int, int);
        bool equal(Coord*);
};

#endif
