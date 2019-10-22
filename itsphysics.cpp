#include "itsphysics.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "createvector.h"

void acceleration_of_object(createVector &currentAcceleration, createVector &dragForce, double &massObject)
{
    double newAccelerationValue;
    for (size_t i{0};i<currentAcceleration.get_size();i++)
    {
        newAccelerationValue = currentAcceleration.get_value(i) + dragForce.get_value(i) / massObject;
        currentAcceleration.set_value(i,newAccelerationValue);
    }
}

void velocity_of_object(createVector &currentVelocity, createVector &acceleration, double &timeStep)
{
    double newVelocityValue;
    for (size_t i{0};i<currentVelocity.get_size();i++)
    {
        newVelocityValue = currentVelocity.get_value(i) + acceleration.get_value(i) * timeStep;
        currentVelocity.set_value(i,newVelocityValue);
    }
}
// overload operators to get rid of loops
void position_of_object(createVector &currentPosition, createVector &velocity, double &timeStep)
{
    double newPositionValue;
    for (size_t i{0};i<currentPosition.get_size();i++)
    {
        newPositionValue = currentPosition.get_value(i) + velocity.get_value(i) * timeStep;
        currentPosition.set_value(i,newPositionValue);
    }
}

void collision_velocity(createVector &currentVelocity, double &coefficientOfRestitution)
{
    double nVelocity;
    for(size_t i{0};i<currentVelocity.get_size();i++)
    {
        nVelocity = currentVelocity.get_value(i)*coefficientOfRestitution;
        currentVelocity.set_value(i,nVelocity);
    }
}
