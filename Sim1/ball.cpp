#include "ball.hh"
#include "constants.hh"

Ball::Ball(PhysicsObject& object, int size, double bouncy): PhysicsObject(object), size_(size), bounciness_(bouncy) {}
Ball::~Ball() {}

int Ball::get_size()
{
    return size_;
}

void Ball::check_collision()
{
    Coord loc = this->get_location();
    Momentum mom = this->get_momentum();

    // floor
    if(loc.y < size_ && mom.y_amount < 0) {
        set_location(loc.x, size_ + 1);
        double new_momentum = -(bounciness_/100.0) * mom.y_amount;
        if(abs(new_momentum) < MINIMUM_BOUNCE) {
            set_rest(true);
            set_momentum(mom.x_amount, 0);
        }
        else {
            set_momentum(mom.x_amount, new_momentum);
        }
    }

    //ceiling
    else if(WINDOW_HEIGHT - loc.y < size_ && mom.y_amount > 0) {
        set_location(loc.x, WINDOW_HEIGHT - size_ - 1);
        double new_momentum = -(bounciness_/100.0) * mom.y_amount;
        if(abs(new_momentum) < MINIMUM_BOUNCE) {
            set_momentum(mom.x_amount, 0);
        }
        else {
            set_momentum(mom.x_amount, new_momentum);
        }
    }

    //left wall
    if(loc.x < size_ && mom.x_amount < 0) {
        set_location(size_, loc.y + 1);
        double new_momentum = -(bounciness_/100.0) * mom.x_amount;
        if(abs(new_momentum) < MINIMUM_BOUNCE) {
            set_momentum(0, mom.y_amount);
        }
        else {
            set_momentum(new_momentum, mom.y_amount);
        }
    }

    // right wall
    else if(WINDOW_WIDTH - loc.x < size_ && mom.x_amount > 0) {
        set_location(WINDOW_WIDTH - size_ - 1, loc.y);
        double new_momentum = -(bounciness_/100.0) * mom.x_amount;
        if(abs(new_momentum) < MINIMUM_BOUNCE) {
            set_momentum(0, mom.y_amount);
        }
        else {
            set_momentum(new_momentum, mom.y_amount);
        }
   }

}
