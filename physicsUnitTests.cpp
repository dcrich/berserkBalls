#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "createvector.h"
#include "itsphysics.h"
#include "objectposition.h"

void expect_equal_stdvectors(std::vector<double> mVector, std::vector<double> mCheckVector)
{
    EXPECT_EQ(mVector[0],mCheckVector[0]);
    EXPECT_EQ(mVector[1],mCheckVector[1]);
    EXPECT_EQ(mVector[2],mCheckVector[2]);
}



TEST(createAnObject, whenObjectCreated_StartAtOrigin)
{
    objectPosition currentPosition;
    std::vector<double> checkPosition{0,0,0};
    expect_equal_stdvectors(currentPosition.return_position_vector(),checkPosition);
}
TEST(createObjectVelcity, whenObjectCreated_ExpectVelocity)
{
    objectPosition currentPosition;
    std::vector<double> checkVelocity{6,0,6};
    expect_equal_stdvectors(currentPosition.get_velocity(), checkVelocity);
}
TEST(updateObjectVelocity, whenVelocityChanges_ExpectNewVelocity)
{
    objectPosition currentPosition;
    std::vector<double> checkVelocity{1,2,3};
    currentPosition.update_velocity(checkVelocity);
    expect_equal_stdvectors(currentPosition.get_velocity(),checkVelocity);
}
TEST(createObjectAcceleration, whenObjectCreated_ExpectAcceleration)
{
    objectPosition currentPosition;
    std::vector<double> checkAcceleration{0,0,-9.81};
    expect_equal_stdvectors(currentPosition.get_acceleration(),checkAcceleration);
}
TEST(updateObjectAcceleration, whenAccelerationChanges_ExpectNewAcceleration)
{
    objectPosition currentPosition;
    std::vector<double> checkAcceleration{1,2,3};
    currentPosition.update_acceleration(checkAcceleration);
    expect_equal_stdvectors(currentPosition.get_acceleration(),checkAcceleration);
}
TEST(createObjectDrag, whenObjectCreated_ExpectDrag)
{
    objectPosition currentPosition;
    std::vector<double> checkDrag{-0.5,-0.5,-0.5};
    expect_equal_stdvectors(currentPosition.get_drag(),checkDrag);
}
TEST(updateObjectDrag, whenDragChanges_ExpectNewDrag)
{
    objectPosition currentPosition;
    std::vector<double> checkDrag{1,2,3};
    currentPosition.update_drag(checkDrag);
    expect_equal_stdvectors(currentPosition.get_drag(),checkDrag);
}
TEST(updateObjectPosition, whenObjectMoves_ExpectNewPosition)
{

}


//TEST(changeInXYZPosition, whenVelocityinXYZ_ExpectXYZChange)
//{
//    createVector currentPosition;
//    createVector currentVelocity;
//    std::vector<double> updateVelocity{10,10,10};
//    currentVelocity.redefine_My_Vector(updateVelocity);
//    double timeStep{1};
//    createVector newPosition = position_of_object(currentPosition,currentVelocity,timeStep);
//    std::vector<double> checkNewPosition{10,10,10};

//    EXPECT_EQ(checkNewPosition[0],newPosition.get_value(0));
//    EXPECT_EQ(checkNewPosition[1],newPosition.get_value(1));
//    EXPECT_EQ(checkNewPosition[2],newPosition.get_value(2));
//}

