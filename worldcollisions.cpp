#include "worldcollisions.h"
#include "math.h"
#include "objectposition.h"
#include "vector"


std::vector<int> check_for_collision(objectPosition *position, float boundary,float sphereRadius)
{
    std::vector<double> coordinates {position->get_position()};
    std::vector<int> checkCollision{0,0,0};
    double upperVirtualBoundary = static_cast<double>(boundary-sphereRadius);
    double lowerVirtualBoundary = static_cast<double>(sphereRadius);

    for (size_t i{0};i<coordinates.size();i++)
    {
        if (coordinates[i]<=lowerVirtualBoundary || coordinates[i]>=upperVirtualBoundary)
        {
            checkCollision[i] = 1;
            if(coordinates[i]<=lowerVirtualBoundary)
            {
                coordinates[i]=lowerVirtualBoundary;
            }
            else
            {
                coordinates[i] = upperVirtualBoundary;
            }
        }
        position->redefine_position(coordinates);
    }

    return checkCollision;
}

bool check_for_goal(objectPosition *position, float boundary, float goalSize)
{
    bool didYouWin{0};
    std::vector<double> coordinates {position->get_position()};
    double VirtualBoundary = static_cast<double>(boundary-goalSize);

    if (coordinates[0]>=VirtualBoundary && coordinates[1]>=VirtualBoundary && coordinates[2]>=VirtualBoundary)
    {
        didYouWin = 1;
        std::vector<double> zeroVelocity{0,0,0};
        position->update_velocity(zeroVelocity);
        std::vector<double> zeroDrag{0,0,0};
        position->update_drag(zeroDrag);
        std::vector<double> zeroAcceleration{0,0,0};
        position->update_acceleration(zeroAcceleration);
    }

    return didYouWin;
}
bool check_for_lose(objectPosition *position, bool didYouLose, float sphereRadius)
{
    std::vector<double> currentVelocity {position->get_velocity()};
    std::vector<double> coordinates {position->get_position()};
    double basicallyZero = .5;

    if (abs(currentVelocity[2])<=basicallyZero && coordinates[2]<=static_cast<double>(sphereRadius))
    {
        didYouLose = true;
    }
    else
    {
        didYouLose = false;
    }
    return didYouLose;
}
