#ifndef SDL2_BASIC_SETUP_JETSKI_H
#define SDL2_BASIC_SETUP_JETSKI_H


#include <SDL_events.h>

class Jetski
{
public:
    static const int jetskiWidth = 130,jetskiHeight = 66;
// linear velocity is the measure of rate of
// change of displacement with respect to time
// when the object moves along a straight path (it is a vector quantity)
    static const int maximumLinearVelocity = 45;

    float jetskiX, jetskiY,velocity,acceleration = 0.025, turnSpeed;


    Jetski();

    void handleEvent(SDL_Event& e);

    void move(int wall);

    SDL_Rect shape;
};


#endif
