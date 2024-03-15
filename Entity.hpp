#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <raylib.h>

class Entity {
public:
    Vector2 position;
    float radius; // Or any other common member variables

    // Declare any common methods or functions
    virtual void Update(float delta_time) = 0;
    virtual void Draw() = 0;
};

#endif 