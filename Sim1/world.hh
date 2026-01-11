#ifndef WORLD_HH
#define WORLD_HH

#include <map>
#include <vector>
#include "ball.hh"

class World
{
public:
    World();
    ~World();

    vector<Ball>& get_balls();
    void add_ball(Type type, Color color, string name, int weight, Coord location, int size, Momentum momentum = {0, 0});

private:
   vector<Ball> balls_;

};

#endif // WORLD_HH
