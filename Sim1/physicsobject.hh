#ifndef PHYSICSOBJECT_HH
#define PHYSICSOBJECT_HH

#include "qnamespace.h"
#include <QColor>
#include <string>

using namespace std;

enum Color {
    WHITE = Qt::white,
    BLACK = Qt::black,
    RED = Qt::red,
    GREEN = Qt::green,
    BLUE = Qt::blue,
    YELLOW = Qt::yellow
};

enum Type {
    STATIC,
    DYNAMIC
};

struct Coord {
    double x;
    double y;
};

struct Momentum {
    double x_amount;
    double y_amount;
};

class PhysicsObject
{
public:
    PhysicsObject(Type type = DYNAMIC, Color color = BLACK, string name = "placeholder", int weight = 10, Coord location = {0, 0}, Momentum momentum = {0, 0});
    ~PhysicsObject();

    virtual Type get_type();
    virtual QColor get_color();
    virtual string get_name();
    virtual int get_weight();
    virtual Coord get_location();
    virtual Momentum get_momentum();

    virtual void update_location();
    virtual void update_momentum();
    virtual void set_location(int x, int y);


private:
    Type type_;
    Color color_;
    string name_;
    int weight_;
    Coord location_;
    Momentum momentum_;
};

#endif // PHYSICSOBJECT_HH
