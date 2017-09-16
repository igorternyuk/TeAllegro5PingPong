#ifndef BALL_H
#define BALL_H
#include "moveable.h"

class Paddle;

class Ball : public Moveable
{
public:
    Ball(int x, int y, int speed, Direction dir, int radius);
    ~Ball();
    int getRadius() const;
    bool isCollision(Paddle *player) const;
    void draw();
private:
    const int radius_;
};

#endif // BALL_H
