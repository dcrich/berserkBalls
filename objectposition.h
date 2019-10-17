#ifndef OBJECTPOSITION_H
#define OBJECTPOSITION_H
#include "createvector.h"
#include <vector>

class objectPosition
{
public:
    objectPosition();
    std::vector<double> return_position_vector();
    std::vector<double> get_velocity();
    void update_velocity(std::vector<double> newVelocity);
    std::vector<double> get_acceleration();
    void update_acceleration(std::vector<double> newAcceleration);
    std::vector<double> get_drag();
    void update_drag(std::vector<double> newDrag);
    double get_mass();
    void update_mass(double newMass);

private:
    createVector currentPosition;
    std::vector<double> currentVelocity{6,0,6};
    std::vector<double> currentAcceleration{0,0,-9.81};
    std::vector<double> currentDrag{-0.5,-0.5,-0.5};
    double currentMass{5};



};

#endif // OBJECTPOSITION_H
