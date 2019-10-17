#include <iostream>
#include "objectposition.h"
#include "createvector.h"

objectPosition::objectPosition()
{
}

std::vector<double> objectPosition::return_position_vector()
{
    return currentPosition.return_vector();
}

std::vector<double> objectPosition::get_velocity()
{
    return currentVelocity;
}

void objectPosition::update_velocity(std::vector<double> newVelocity)
{
    currentVelocity = newVelocity;
}

std::vector<double> objectPosition::get_acceleration()
{
    return currentAcceleration;
}
void objectPosition::update_acceleration(std::vector<double> newAcceleration)
{
    currentAcceleration = newAcceleration;
}
std::vector<double> objectPosition::get_drag()
{
    return currentDrag;
}
void objectPosition::update_drag(std::vector<double> newDrag)
{
    currentDrag = newDrag;
}
double objectPosition::get_mass()
{
    return currentMass;
}
void objectPosition::update_mass(double newMass)
{
    currentMass = newMass;
}
