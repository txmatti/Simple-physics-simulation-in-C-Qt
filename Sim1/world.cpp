#include "world.hh"

World::World() {}

World::~World()
{}

vector<Ball>& World::get_balls()
{
    return balls_;
}

void World::add_ball(Type type, Color color, string name, int weight, Coord location, int size, Momentum momentum)
{
    PhysicsObject ball_object = {type, color, name, weight, location, momentum};
    Ball new_ball = {ball_object, size};
    balls_.push_back(new_ball);
}


