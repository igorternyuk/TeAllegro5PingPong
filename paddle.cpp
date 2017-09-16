#include "paddle.h"
#include "ball.h"

Paddle::Paddle(int x, int y, int speed, Direction dir, int width, int height) :
    Moveable(x, y, speed, dir), width_(width), height_(height), goals_(0)
{}

Paddle::~Paddle()
{}

int Paddle::getWidth() const
{
    return width_;
}

int Paddle::getHeight() const
{
    return height_;
}

int Paddle::getScore() const
{
    return goals_;
}

void Paddle::moveCPU(Ball *ball, const int &topLimit, const int &bottomLimit)
{
    Direction ballDir = ball->getDirection();
    if(ballDir == Direction::RIGHT_UP)
    {
        if(getY() - getSpeed() >= topLimit)
        {
            this->setDirection(Direction::UP);
            this->move();
        }
        else
            this->setDirection(Direction::STOP);
    }
    else if(ballDir == Direction::RIGHT_DOWN)
    {
        if(getY() + getSpeed() <= bottomLimit - getHeight())
        {
            this->setDirection(Direction::DOWN);
            this->move();
        }
        else
            this->setDirection(Direction::STOP);
    }
}

void Paddle::reset()
{
    resetPosition();
    goals_ = 0;
}

void Paddle::incScore()
{
    ++goals_;
}

void Paddle::draw()
{
    al_draw_filled_rectangle(getX(), getY(), getX() + width_, getY() + height_, al_map_rgb(100, 100, 200));
}
