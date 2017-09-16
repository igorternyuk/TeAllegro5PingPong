#include "ball.h"
#include "paddle.h"

Ball::Ball(int x, int y, int speed, Direction dir, int radius) :
    Moveable(x, y, speed, dir), radius_(radius)
{}

Ball::~Ball()
{}

int Ball::getRadius() const
{
    return radius_;
}

bool Ball::isCollision(Paddle *player) const
{
    int px = player->getX(), py = player->getY();
    int pw = player->getWidth(), ph = player->getHeight();
    return getX() >= px - radius_ && getX() - px - pw <= radius_ &&
           getY() >= py - radius_ && getY() - py - ph <= radius_;
}

void Ball::draw()
{
    al_draw_filled_ellipse(getX(), getY(), radius_, radius_, al_map_rgb(255, 0, 0));
}
