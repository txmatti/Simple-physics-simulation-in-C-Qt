#ifndef BALL_HH
#define BALL_HH

#include "physicsobject.hh"

class Ball:public PhysicsObject
{
public:
    Ball(PhysicsObject& object, int size);
    ~Ball();

    int get_size();
    void check_collision();

private:
    int size_;

};

#endif // BALL_HH
