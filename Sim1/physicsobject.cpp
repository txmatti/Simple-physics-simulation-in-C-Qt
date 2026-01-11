#include "physicsobject.hh"
#include "constants.hh"

PhysicsObject::PhysicsObject(Type type, Color color, string name, int weight, Coord location, Momentum momentum)
    : type_(type), color_(color), name_(name), weight_(weight), location_(location), momentum_(momentum)
{

}

PhysicsObject::~PhysicsObject() {}


Type PhysicsObject::get_type() const
{
    return type_;
}

QColor PhysicsObject::get_color() const
{
    return QColor(static_cast<Qt::GlobalColor>(color_));
}

string PhysicsObject::get_name() const
{
    return name_;
}

int PhysicsObject::get_weight() const
{
    return weight_;
}

Coord PhysicsObject::get_location() const
{
    return location_;
}

Momentum PhysicsObject::get_momentum() const
{
    return momentum_;
}

void PhysicsObject::update_location()
{
    location_.x += momentum_.x_amount * (TICK_DURATION / 1000.0);
    location_.y += momentum_.y_amount * (TICK_DURATION / 1000.0);
}

void PhysicsObject::apply_gravity()
{
    if(!at_rest_) {
        momentum_.y_amount -= GRAVITY * (TICK_DURATION / 1000.0);
    }
}

void PhysicsObject::set_location(int x, int y)
{
    location_.x = x;
    location_.y = y;
}

void PhysicsObject::set_momentum(int x, int y)
{
    momentum_.x_amount = x;
    momentum_.y_amount = y;
}

void PhysicsObject::set_rest(bool value)
{
    at_rest_ = value;
}



