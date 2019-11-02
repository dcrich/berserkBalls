#include <iostream>
#include "objectposition.h"
#include "createvector.h"
#include "itsphysics.h"
#include <math.h>

objectPosition::objectPosition()
{
    currentVelocity.redefine_My_Vector({0,0,0});
    currentAcceleration.redefine_My_Vector({0.0,0.0,0.0});
    currentDrag.redefine_My_Vector({0.0,0.0,0.0});
}

void objectPosition::redefine_position(std::vector<double> newPosition)
{
    currentPosition.redefine_My_Vector(newPosition);
}

void objectPosition::first_position()
{
    acceleration_of_object(currentAcceleration,currentDrag,currentMass);
    velocity_of_object(currentVelocity,currentAcceleration,timeStep);
    formerPosition = currentPosition;
    position_of_object(currentPosition,currentVelocity,timeStep);
}
void objectPosition::update_position()
{
    acceleration_of_object(currentAcceleration,currentDrag,currentMass);
    velocity_of_object(currentVelocity,currentAcceleration,timeStep);
    formerPosition = currentPosition;
    position_of_object(currentPosition,currentVelocity,timeStep);
}

std::vector<double> objectPosition::get_position()
{
    return currentPosition.return_vector();
}

std::vector<double> objectPosition::get_velocity()
{
    std::vector<double> currentVelocityVector = currentVelocity.return_vector();
    return currentVelocityVector;
}

void objectPosition::update_velocity(std::vector<double> newVelocity)
{
    currentVelocity.redefine_My_Vector(newVelocity);
}

std::vector<double> objectPosition::get_acceleration()
{
    std::vector<double> currentAccelerationVector = currentAcceleration.return_vector();
    return currentAccelerationVector;
}
void objectPosition::update_acceleration(std::vector<double> newAcceleration)
{
    currentAcceleration.redefine_My_Vector(newAcceleration);
}
std::vector<double> objectPosition::get_drag()
{
    std::vector<double> currentDragVector = currentDrag.return_vector();
    return currentDragVector;
}
void objectPosition::update_drag(std::vector<double> newDrag)
{
    currentDrag.redefine_My_Vector(newDrag);
}
double objectPosition::get_mass()
{
    return currentMass;
}
void objectPosition::update_mass(double newMass)
{
    double proximityToZero = newMass-0.0;
    if (fabs(proximityToZero) < 0.0001)
    {
        currentMass = 1.0;
    }
    else if (newMass<0.0)
    {
        currentMass = newMass*(-1.0);
    }
    else
    {
        currentMass = newMass;
    }
}
double objectPosition::get_timeStep()
{
    return timeStep;
}
void objectPosition::update_timeStep(double newTimeStep)
{
    timeStep = newTimeStep;
}

void objectPosition::static_collision(size_t normalDirection)
{
    collision_velocity(currentVelocity,coefficientOfRestitution, normalDirection);
}

 objectPosition::~objectPosition()
 {

 }
