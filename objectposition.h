#ifndef OBJECTPOSITION_H
#define OBJECTPOSITION_H
#include "createvector.h"
#include <vector>

class objectPosition
{
public:
    objectPosition();
    void redefine_position(std::vector<double> newPosition);
    void first_position();
    void update_position();
    std::vector<double> get_position();
    std::vector<double> get_velocity();
    void update_velocity(std::vector<double> newVelocity);
    std::vector<double> get_acceleration();
    void update_acceleration(std::vector<double> newAcceleration);
    std::vector<double> get_drag();
    void update_drag(std::vector<double> newDrag);
    double get_mass();
    void update_mass(double newMass);
    double get_timeStep();
    void update_timeStep(double newTimeStep);

    void static_collision(size_t normalDirection);
    ~objectPosition();

private:
    createVector currentPosition;
    createVector formerPosition;
    createVector currentVelocity;
    createVector currentAcceleration;
    createVector currentDrag;
    double currentMass{5.0}; // add function so sphere volume is based on density and mass
    double timeStep{1.0/30.0};
    double coefficientOfRestitution{-0.5};
};

#endif // OBJECTPOSITION_H
