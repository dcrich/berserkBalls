#include "itsphysics.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "createvector.h"



createVector acceleration_of_object(createVector currentAcceleration, createVector dragForce, double massObject)
{
    //a'=a+Fd/m
    createVector newAcceleration;
    double newAccelerationValue;
    for (size_t i{0};i<newAcceleration.get_size();i++)
    {
        newAccelerationValue = currentAcceleration.get_value(i) + dragForce.get_value(i) / massObject;
        newAcceleration.set_value(i,newAccelerationValue);
    }
    return newAcceleration;
}

createVector velocity_of_object(createVector currentVelocity, createVector acceleration, double timeStep)
{
    //v'=v+a*dt
    createVector newVelocity;
    double newVelocityValue;
    for (size_t i{0};i<newVelocity.get_size();i++)
    {
        newVelocityValue = currentVelocity.get_value(i) + acceleration.get_value(i) * timeStep;
        newVelocity.set_value(i,newVelocityValue);
    }
    return newVelocity;
}

createVector position_of_object(createVector currentPosition, createVector velocity, double timeStep)
{
    createVector newPosition;
    double newPositionValue;
    for (size_t i{0};i<newPosition.get_size();i++)
    {
        newPositionValue = currentPosition.get_value(i) + velocity.get_value(i) * timeStep;
        newPosition.set_value(i,newPositionValue);
    }
    return newPosition;
}

createVector collision_velocity(createVector currentVelocity, double coefficientOfRestitution)
{
    createVector newVelocity;
    for(size_t i{0};i<newVelocity.get_size();i++)
    {
        newVelocity.set_value(i,currentVelocity.get_value(i)*coefficientOfRestitution);
    }
    return newVelocity;
}
