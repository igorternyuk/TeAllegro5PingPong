#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "utils.h"

class Moveable
{
public:
   explicit Moveable(int x, int y, int speed, Direction dir);
   virtual ~Moveable();
   int getX() const;
   int getY() const;
   void setDirection(Direction dir);
   int getSpeed() const;
   Direction getDirection() const;
   void setPosition(int x, int y, Direction dir);
   void printInfo() const;
   void move();
   void resetPosition();
   virtual void draw() = 0;
private:
    int x_, y_;
    int initial_x_, initial_y_;
    Direction initialDir_;
    int speed_;
    Direction dir_;
    int dx_, dy_;
};

#endif // MOVEABLE_H
