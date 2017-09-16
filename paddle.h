#ifndef PADDLE_H
#define PADDLE_H
#include "moveable.h"

class Ball;
class Paddle : public Moveable
{
public:
    Paddle(int x, int y, int speed, Direction dir, int width, int height);
    ~Paddle();
    int getWidth() const;
    int getHeight() const;
    int getScore() const;
    void moveCPU(Ball *ball, const int &topLimit, const int &bottomLimit);
    void reset();
    void incScore();
    void draw();
private:
    const int width_;
    const int height_;
    int goals_;
};

#endif // PADDLE_H
