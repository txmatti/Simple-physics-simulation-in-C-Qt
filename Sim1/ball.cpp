#include "ball.hh"

Ball::Ball(PhysicsObject& object, int size): PhysicsObject(object), size_(size) {}
Ball::~Ball() {}

int Ball::get_size()
{
    return size_;
}
