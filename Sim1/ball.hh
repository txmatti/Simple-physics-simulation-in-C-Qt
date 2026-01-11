#ifndef BALL_HH
#define BALL_HH

#include "physicsobject.hh"

class Ball:public PhysicsObject
{
public:
    Ball(PhysicsObject& object, int size, double bouncy = 80.0);
    ~Ball();

    int get_size();
    void check_collision();

private:
    int size_;
    double bounciness_;

};

#endif // BALL_HH
