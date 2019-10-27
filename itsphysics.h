#ifndef ITSPHYSICS_H
#define ITSPHYSICS_H
#include <iostream>
#include "createvector.h"

    void acceleration_of_object(createVector &currentAcceleration, createVector &dragAcceleration, double &massObject);
    void velocity_of_object(createVector &currentVelocity, createVector &acceleration, double &timeStep);
    void position_of_object(createVector &currentPosition, createVector &velocity, double &timeStep);
    void collision_velocity(createVector &currentVelocity, double &coefficientOfRestitution, size_t normalDirection);

#endif // ITSPHYSICS_H
