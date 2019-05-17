#ifndef ROOM_H
#define ROOM_H

#include "labquest.h"

using namespace std;

class Room
{
    private:
        int y;
        int x;
        int width;
        int height;

    public:
        Room(int, int, int, int);
        Room();

        void setY(int);
        int getY();
        void setX(int);
        int getX();
        void setWidth(int);
        int getWidth();
        void setHeight(int);
        int getHeight();
};//Room class

#endif
