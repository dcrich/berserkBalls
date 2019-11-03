#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "createvector.h"
#include "itsphysics.h"
#include "objectposition.h"
#include "worldcollisions.h"

void expect_equal_stdvectors(std::vector<double> mVector, std::vector<double> mCheckVector)
{
    EXPECT_DOUBLE_EQ(mVector[0],mCheckVector[0]);
    EXPECT_DOUBLE_EQ(mVector[1],mCheckVector[1]);
    EXPECT_DOUBLE_EQ(mVector[2],mCheckVector[2]);
}


TEST(createAnObject, whenObjectCreated_StartAtOrigin)
{
    objectPosition currentPosition;
    std::vector<double> checkPosition{0,0,0};
    expect_equal_stdvectors(currentPosition.get_position(),checkPosition);
}
TEST(createObjectVelcity, whenObjectCreated_ExpectVelocity)
{
    objectPosition currentPosition;
    std::vector<double> checkVelocity{0,0,0};
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
    std::vector<double> checkAcceleration{0,0,0};
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
    std::vector<double> checkDrag{0.0,0.0,0.0};
    expect_equal_stdvectors(currentPosition.get_drag(),checkDrag);
}
TEST(updateObjectDrag, whenDragChanges_ExpectNewDrag)
{
    objectPosition currentPosition;
    std::vector<double> checkDrag{1,2,3};
    currentPosition.update_drag(checkDrag);
    expect_equal_stdvectors(currentPosition.get_drag(),checkDrag);
}
TEST(createObjectMass, whenObjectCreated_ExpectMass)
{
    objectPosition currentPosition;
    double checkMass{5.0};
    EXPECT_DOUBLE_EQ(currentPosition.get_mass(),checkMass);
}
TEST(updateObjectMass, whenMassChanges_ExpectNewMass)
{
    objectPosition currentPosition;
    double checkMass{66.0};
    currentPosition.update_mass(checkMass);
    EXPECT_DOUBLE_EQ(currentPosition.get_mass(),checkMass);
}
TEST(avoidZeroMass,whenMassSetToZero_ExpectMassEqualOne)
{
    objectPosition currentPosition;
    double newMass{0.0};
    double checkMass{1.0};
    currentPosition.update_mass(newMass);
    EXPECT_DOUBLE_EQ(currentPosition.get_mass(),checkMass);
}
TEST(avoidNearZeroMass,whenMassSetToZero_ExpectMassEqualOne)
{
    objectPosition currentPosition;
    double newMass{0.00009};
    double checkMass{1.0};
    currentPosition.update_mass(newMass);
    EXPECT_DOUBLE_EQ(currentPosition.get_mass(),checkMass);
}
TEST(avoidNegativeMass,whenMassSetToNegative_ExpectMassEqualAbsNewMass)
{
    objectPosition currentPosition;
    double newMass{-66.0};
    double checkMass{66.0};
    currentPosition.update_mass(newMass);
    EXPECT_DOUBLE_EQ(currentPosition.get_mass(),checkMass);
}
TEST(createObjectTimeStep, whenObjectCreated_ExpectTimeStep)
{
    objectPosition currentPosition;
    double checkTimeStep{1.0/30.0};
    EXPECT_DOUBLE_EQ(currentPosition.get_timeStep(),checkTimeStep);
}
TEST(updateObjectTimeStep, whenTimeStepChanges_ExpectNewTimeStep)
{
    objectPosition currentPosition;
    double checkTimeStep{1.0/400.0};
    currentPosition.update_timeStep(checkTimeStep);
    EXPECT_DOUBLE_EQ(currentPosition.get_timeStep(),checkTimeStep);
}

TEST(firstObjectPositionNoDragNoAccel, whenObjectMoves_ExpectNewPosition)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    std::vector<double> updateAccel{0.0,0.0,0.0};
    currentPosition.update_acceleration(updateAccel);
    std::vector<double> updateDrag{0.0,0.0,0.0};
    currentPosition.update_drag(updateDrag);
    double timeStep{0.5};
    currentPosition.update_timeStep(timeStep);
    currentPosition.first_position();
    std::vector<double> checkNewPosition{5.0,5.0,5.0};
    expect_equal_stdvectors(currentPosition.get_position(),checkNewPosition);
}
TEST(firstObjectPositionNoDrag, whenObjectMoves_ExpectNewPosition)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    std::vector<double> updateAccel{0.0,0.0,-9.8};
    currentPosition.update_acceleration(updateAccel);
    double timeStep{0.5};
    currentPosition.update_timeStep(timeStep);
    currentPosition.first_position();
    std::vector<double> checkNewPosition{5.0,5.0,2.55};
    expect_equal_stdvectors(currentPosition.get_position(),checkNewPosition);
}
TEST(firstObjectPositionPositiveDragNegativeAccel, whenObjectMoves_ExpectNewPosition)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    std::vector<double> updateAccel{0.0,0.0,-9.8};
    currentPosition.update_acceleration(updateAccel);
    std::vector<double> updateDrag{1.0,2.0,3.0};
    currentPosition.update_drag(updateDrag);
    double timeStep{0.5};
    currentPosition.update_timeStep(timeStep);
    currentPosition.first_position();
    std::vector<double> checkNewPosition{5.05,5.1,2.7};
    expect_equal_stdvectors(currentPosition.get_position(),checkNewPosition);
}
TEST(firstObjectPositionNegativeDragNegativeAccel, whenObjectMoves_ExpectNewPosition)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    std::vector<double> updateAccel{0.0,0.0,-9.8};
    currentPosition.update_acceleration(updateAccel);
    std::vector<double> updateDrag{-1.0,-2.0,-3.0};
    currentPosition.update_drag(updateDrag);
    double timeStep{0.5};
    currentPosition.update_timeStep(timeStep);
    currentPosition.update_position();
    std::vector<double> checkNewPosition{4.95,4.9,2.4};
    expect_equal_stdvectors(currentPosition.get_position(),checkNewPosition);
}
TEST(firstObjectPositionNegativeDragPositiveAccel, whenObjectMoves_ExpectNewPosition)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    std::vector<double> updateDrag{-1.0,-2.0,-3.0};
    currentPosition.update_drag(updateDrag);
    std::vector<double> updateAccel{1.0,2.0,3.0};
    currentPosition.update_acceleration(updateAccel);
    double timeStep{0.5};
    currentPosition.update_timeStep(timeStep);
    currentPosition.first_position();
    std::vector<double> checkNewPosition{5.2,5.4,5.6};
    expect_equal_stdvectors(currentPosition.get_position(),checkNewPosition);
}
TEST(firstObjectPositionMixedSignDragMixedSignAccel, whenObjectMoves_ExpectNewPosition)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    std::vector<double> updateDrag{10.0,-2.0,-3.0};
    currentPosition.update_drag(updateDrag);
    std::vector<double> updateAccel{100.0,-600.0,-9.8};
    currentPosition.update_acceleration(updateAccel);
    double timeStep{0.5};
    currentPosition.update_timeStep(timeStep);
    currentPosition.first_position();
    std::vector<double> checkNewPosition{30.5,-145.1,2.4};
    expect_equal_stdvectors(currentPosition.get_position(),checkNewPosition);
}
TEST(startVelocityCollisionWithStatic, whenCollision_ExpectChangeInVelocity)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    currentPosition.first_position();
    currentPosition.static_collision(2);
    std::vector<double> checkVelocity{10,10,-5};
    expect_equal_stdvectors(currentPosition.get_velocity(),checkVelocity);
}
TEST(collisionWithXYzeroPlane, whenBallMeetsGround_ExpectCollision)
{
    objectPosition currentPosition;

}

TEST(collisionWithXZzeroPlane, whenBallMeetsWall_ExpectCollision)
{
    objectPosition currentPosition;

}
TEST(collisionWithYZzeroPlane, whenBallMeetsWall_ExpectCollision)
{
    objectPosition currentPosition;

}
TEST(collisionWithXYzPlane, whenBallMeetsGround_ExpectCollision)
{
    objectPosition currentPosition;

}

TEST(collisionWithXZyPlane, whenBallMeetsWall_ExpectCollision)
{
    objectPosition currentPosition;

}
TEST(collisionWithYZxPlane, whenBallMeetsWall_ExpectCollision)
{
    objectPosition currentPosition;

}

TEST(checkLoseWhenBallStopsMovingInZ, whenBallLosesVerticalVelocity_ExpectLose)
{

}
TEST(checkLoseWhenBallMovingPositiveInZ, whenBallHasVerticalVelocity_ExpectNoLose)
{

}
TEST(checkLoseWhenBallMovingNegativeInZ, whenBallHasVerticalVelocity_ExpectNoLose)
{

}

TEST(checkIfBallIsInGoal, whenBallInGoal_ExpectWin)
{

}
TEST(checkIfBallNotInGoal, whenBallNotInGoal_ExpectNoWin)
{

}

TEST(osgVecFromStdVector, whenConvertfromVectorToVec_ExpectValidTransfer)
{

}

