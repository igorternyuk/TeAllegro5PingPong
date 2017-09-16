#include "moveable.h"

Moveable::Moveable(int x, int y, int speed, Direction dir) :
    x_(x), y_(y),initial_x_(x), initial_y_(y), initialDir_(dir), speed_(speed)
{
   setDirection(dir);
}

Moveable::~Moveable()
{}

int Moveable::getX() const
{
    return x_;
}

int Moveable::getY() const
{
    return y_;
}

void Moveable::setDirection(Direction dir)
{
    dir_ = dir;
    switch(dir_)
    {
        case Direction::STOP :
            dx_ = 0;
            dy_ = 0;
            break;
        case Direction::UP :
            dx_ = 0;
            dy_ = -speed_;
            break;
        case Direction::DOWN :
            dx_ = 0;
            dy_ = +speed_;
            break;
        case Direction::LEFT_UP :
            dx_ = -speed_;
            dy_ = -speed_;
            break;
        case Direction::LEFT_DOWN :
            dx_ = -speed_;
            dy_ = +speed_;
            break;
        case Direction::RIGHT_UP :
            dx_ = +speed_;
            dy_ = -speed_;
            break;
        case Direction::RIGHT_DOWN :
            dx_ = +speed_;
            dy_ = +speed_;
            break;
    }
}

int Moveable::getSpeed() const
{
    return speed_;
}

Direction Moveable::getDirection() const
{
    return dir_;
}

void Moveable::setPosition(int x, int y, Direction dir)
{
    x_ = x;
    y_ = y;
    setDirection(dir);
}

void Moveable::printInfo() const
{
    std::cout << "Координаты: " << std::endl;
    std::cout << "X = " << getX() << " Y = " << getY() << std::endl;
    switch(dir_)
    {
        case Direction::STOP :
            std::cout << "Направление - стоп " << std::endl;
            break;
        case Direction::UP :
            std::cout << "Направление - вверх " << std::endl;
            break;
        case Direction::DOWN :
            std::cout << "Направление - вниз" << std::endl;
            break;
        case Direction::LEFT_UP :
            std::cout << "Направление - влево вверх " << std::endl;
            break;
        case Direction::LEFT_DOWN :
            std::cout << "Направление - влево вниз " << std::endl;
            break;
        case Direction::RIGHT_UP :
            std::cout << "Направление - вправо вверх " << std::endl;
            break;
        case Direction::RIGHT_DOWN :
            std::cout << "Направление - вправо вниз " << std::endl;
            break;
    }
}

void Moveable::move()
{
    x_ += dx_;
    y_ += dy_;
}

void Moveable::resetPosition()
{
    x_ = initial_x_;
    y_ = initial_y_;
    dir_ = initialDir_;
}
