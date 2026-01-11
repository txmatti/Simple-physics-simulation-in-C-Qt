#include "ball.hh"
#include "constants.hh"

Ball::Ball(PhysicsObject& object, int size): PhysicsObject(object), size_(size) {}
Ball::~Ball() {}

int Ball::get_size()
{
    return size_;
}

void Ball::check_collision()
{
    Coord loc = this->get_location();
    Momentum mom = this->get_momentum();
    if(loc.y < size_ || WINDOW_HEIGHT - loc.y < size_) {
        set_momentum(mom.x_amount, -mom.y_amount);
    }
    if (loc.x < size_ || WINDOW_WIDTH - loc.x < size_) {
        set_momentum(-mom.x_amount, mom.y_amount);

    }


}
