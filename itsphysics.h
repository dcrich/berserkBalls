#ifndef ITSPHYSICS_H
#define ITSPHYSICS_H
#include <iostream>
#include "createvector.h"

    createVector acceleration_of_object(createVector currentAcceleration, createVector dragAcceleration, double massObject);
    createVector velocity_of_object(createVector currentVelocity, createVector acceleration, double timeStep);
    createVector position_of_object(createVector currentPosition, createVector velocity, double timeStep);
    createVector collision_velocity(createVector currentVelocity, double coefficientOfRestitution);

#endif // ITSPHYSICS_H
