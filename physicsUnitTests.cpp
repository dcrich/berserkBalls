#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <osg/Vec3>
#include "createvector.h"
#include "itsphysics.h"
#include "objectposition.h"
#include "worldcollisions.h"
#include "stdtoosgadapt.h"


void expect_equal_stdvector_double(std::vector<double> mVector, std::vector<double> mCheckVector)
{
    EXPECT_DOUBLE_EQ(mVector[0],mCheckVector[0]);
    EXPECT_DOUBLE_EQ(mVector[1],mCheckVector[1]);
    EXPECT_DOUBLE_EQ(mVector[2],mCheckVector[2]);
}
void expect_equal_stdvector_int(std::vector<int> mVector, std::vector<int> mCheckVector)
{
    EXPECT_DOUBLE_EQ(mVector[0],mCheckVector[0]);
    EXPECT_DOUBLE_EQ(mVector[1],mCheckVector[1]);
    EXPECT_DOUBLE_EQ(mVector[2],mCheckVector[2]);
}


TEST(createAnObject, whenObjectCreated_StartAtOrigin)
{
    objectPosition currentPosition;
    std::vector<double> checkPosition{0,0,0};
    expect_equal_stdvector_double(currentPosition.get_position(),checkPosition);
}
TEST(createObjectVelcity, whenObjectCreated_ExpectVelocity)
{
    objectPosition currentPosition;
    std::vector<double> checkVelocity{0,0,0};
    expect_equal_stdvector_double(currentPosition.get_velocity(), checkVelocity);
}
TEST(updateObjectVelocity, whenVelocityChanges_ExpectNewVelocity)
{
    objectPosition currentPosition;
    std::vector<double> checkVelocity{1,2,3};
    currentPosition.update_velocity(checkVelocity);
    expect_equal_stdvector_double(currentPosition.get_velocity(),checkVelocity);
}
TEST(createObjectAcceleration, whenObjectCreated_ExpectAcceleration)
{
    objectPosition currentPosition;
    std::vector<double> checkAcceleration{0,0,0};
    expect_equal_stdvector_double(currentPosition.get_acceleration(),checkAcceleration);
}
TEST(updateObjectAcceleration, whenAccelerationChanges_ExpectNewAcceleration)
{
    objectPosition currentPosition;
    std::vector<double> checkAcceleration{1,2,3};
    currentPosition.update_acceleration(checkAcceleration);
    expect_equal_stdvector_double(currentPosition.get_acceleration(),checkAcceleration);
}
TEST(createObjectDrag, whenObjectCreated_ExpectDrag)
{
    objectPosition currentPosition;
    std::vector<double> checkDrag{0.0,0.0,0.0};
    expect_equal_stdvector_double(currentPosition.get_drag(),checkDrag);
}
TEST(updateObjectDrag, whenDragChanges_ExpectNewDrag)
{
    objectPosition currentPosition;
    std::vector<double> checkDrag{1,2,3};
    currentPosition.update_drag(checkDrag);
    expect_equal_stdvector_double(currentPosition.get_drag(),checkDrag);
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
    expect_equal_stdvector_double(currentPosition.get_position(),checkNewPosition);
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
    expect_equal_stdvector_double(currentPosition.get_position(),checkNewPosition);
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
    expect_equal_stdvector_double(currentPosition.get_position(),checkNewPosition);
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
    expect_equal_stdvector_double(currentPosition.get_position(),checkNewPosition);
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
    expect_equal_stdvector_double(currentPosition.get_position(),checkNewPosition);
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
    expect_equal_stdvector_double(currentPosition.get_position(),checkNewPosition);
}
TEST(startVelocityCollisionWithStatic, whenCollision_ExpectChangeInVelocity)
{
    objectPosition currentPosition;
    std::vector<double> updateVelocity{10.0,10.0,10.0};
    currentPosition.update_velocity(updateVelocity);
    currentPosition.first_position();
    currentPosition.static_collision(2);
    std::vector<double> checkVelocity{10,10,-5};
    expect_equal_stdvector_double(currentPosition.get_velocity(),checkVelocity);
}
TEST(collisionWithXYzeroPlane, whenBallMeetsGround_ExpectCollision)
{
    float sphereRadius{5.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{10,10,static_cast<double>(sphereRadius)};
    currentPosition->redefine_position(newPosition);
    float boundary{100.f};
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{0,0,1};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(collisionWithXZzeroPlane, whenBallMeetsWall_ExpectCollision)
{
    float sphereRadius{5.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{10,static_cast<double>(sphereRadius),10};
    currentPosition->redefine_position(newPosition);
    float boundary{100.f};
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{0,1,0};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(collisionWithYZzeroPlane, whenBallMeetsWall_ExpectCollision)
{
    float sphereRadius{5.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(sphereRadius),10,10};
    currentPosition->redefine_position(newPosition);
    float boundary{100.f};
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{1,0,0};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(collisionWithXYzPlane, whenBallMeetsGround_ExpectCollision)
{
    float sphereRadius{5.f};
    float boundary{100.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{10,10,static_cast<double>(boundary-sphereRadius)};
    currentPosition->redefine_position(newPosition);
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{0,0,1};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(collisionWithXZyPlane, whenBallMeetsWall_ExpectCollision)
{
    float sphereRadius{5.f};
    float boundary{100.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{10,static_cast<double>(boundary-sphereRadius),10};
    currentPosition->redefine_position(newPosition);
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{0,1,0};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(collisionWithYZxPlane, whenBallMeetsWall_ExpectCollision)
{
    float sphereRadius{5.f};
    float boundary{100.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(boundary-sphereRadius),10,10};
    currentPosition->redefine_position(newPosition);
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{1,0,0};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(collisionWithEdge, whenBallMeetsWall_ExpectCollision)
{
    float sphereRadius{5.f};
    float boundary{100.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(boundary-sphereRadius),static_cast<double>(boundary-sphereRadius),10};
    currentPosition->redefine_position(newPosition);
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{1,1,0};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(collisionWithCorner, whenBallMeetsWall_ExpectCollision)
{
    float sphereRadius{5.f};
    float boundary{100.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(sphereRadius),static_cast<double>(sphereRadius),static_cast<double>(sphereRadius)};
    currentPosition->redefine_position(newPosition);
    std::vector<int> collisionCheck = check_for_collision(currentPosition, boundary, sphereRadius);
    std::vector<int> checkValue{1,1,1};
    expect_equal_stdvector_int(collisionCheck,checkValue);
    delete currentPosition;
}
TEST(checkIfBallIsInGoal, whenBallInGoal_ExpectWin)
{
    float goalsize{5.f};
    float boundary{100.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(boundary-goalsize),static_cast<double>(boundary-goalsize),static_cast<double>(boundary-goalsize)};
    currentPosition->redefine_position(newPosition);
    bool goalCheck = check_for_goal(currentPosition, boundary, goalsize);
    ASSERT_TRUE(goalCheck);
    delete currentPosition;
}
TEST(checkIfBallNotInGoal, whenBallNotInGoal_ExpectNoWin)
{
    float goalsize{5.f};
    float boundary{100.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(goalsize),static_cast<double>(goalsize),static_cast<double>(goalsize)};
    currentPosition->redefine_position(newPosition);
    bool goalCheck = check_for_goal(currentPosition, boundary, goalsize);
    ASSERT_FALSE(goalCheck);
    delete currentPosition;
}
TEST(checkLoseWhenBallStopsMovingInZAndZpositionZero, whenBallLosesVerticalVelocity_ExpectLose)
{
    float sphereRadius{5.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(sphereRadius),static_cast<double>(sphereRadius),static_cast<double>(sphereRadius)};
    currentPosition->redefine_position(newPosition);
    std::vector<double> objectVelocity{10,10,0};
    currentPosition->update_velocity(objectVelocity);
    bool loseCheck{0};
    loseCheck= check_for_lose(currentPosition, loseCheck, sphereRadius);
    ASSERT_TRUE(loseCheck);
    delete currentPosition;
}
TEST(checkLoseWhenBallStopsMovingInZAndZpositionNotZero, whenBallLosesVerticalVelocity_ExpectLose)
{
    float sphereRadius{5.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(2*sphereRadius),static_cast<double>(2*sphereRadius),static_cast<double>(2*sphereRadius)};
    currentPosition->redefine_position(newPosition);
    std::vector<double> objectVelocity{10,10,0};
    currentPosition->update_velocity(objectVelocity);
    bool loseCheck{0};
    loseCheck= check_for_lose(currentPosition, loseCheck, sphereRadius);
    ASSERT_FALSE(loseCheck);
    delete currentPosition;
}
TEST(checkLoseWhenBallMovingPositiveInZ, whenBallHasVerticalVelocity_ExpectNoLose)
{
    float sphereRadius{5.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(sphereRadius),static_cast<double>(sphereRadius),static_cast<double>(sphereRadius)};
    currentPosition->redefine_position(newPosition);
    std::vector<double> objectVelocity{10,10,10};
    currentPosition->update_velocity(objectVelocity);
    bool loseCheck{0};
    loseCheck= check_for_lose(currentPosition, loseCheck, sphereRadius);
    ASSERT_FALSE(loseCheck);
    delete currentPosition;
}
TEST(checkLoseWhenBallMovingNegativeInZ, whenBallHasVerticalVelocity_ExpectNoLose)
{
    float sphereRadius{5.f};
    objectPosition * currentPosition = new objectPosition;
    std::vector<double> newPosition{static_cast<double>(sphereRadius),static_cast<double>(sphereRadius),static_cast<double>(sphereRadius)};
    currentPosition->redefine_position(newPosition);
    std::vector<double> objectVelocity{10,10,-10};
    currentPosition->update_velocity(objectVelocity);
    bool loseCheck{0};
    loseCheck= check_for_lose(currentPosition, loseCheck, sphereRadius);
    ASSERT_FALSE(loseCheck);
    delete currentPosition;
}
TEST(osgVecFromStdVector, whenConvertfromVectorToVec_ExpectValidTransfer)
{
    std::vector<double> mVector{1,2,3};
    osg::Vec3 newVec3 = convert_physics_vector_to_vec3(mVector);
    EXPECT_EQ(newVec3[0],mVector[0]);
    EXPECT_EQ(newVec3[1],mVector[1]);
    EXPECT_EQ(newVec3[2],mVector[2]);
}

