#include "gtest/gtest.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "createvector.h"
#include "itsphysics.h"
#include "objectposition.h"


//TEST(doesAccelerationWork, whenAccelerationCalculated_ExpectNewAcceleration)
//{
//    createVector currentAcceleration;
//    std::vector<double> accelerationVector{1,1,1};
//    createVector dragAcceleration;
//    std::vector<double> dragVector{1,1,1};
//    dragAcceleration.redefine_My_Vector(dragVector);
//    double massObject{5};
//    acceleration_of_object(currentAcceleration,dragAcceleration,massObject);

//}
void expectEqualVectors(createVector mVector, std::vector<double> mCheckVector)
{

    EXPECT_EQ(mVector.get_value(0),mCheckVector[0]);
    EXPECT_EQ(mVector.get_value(1),mCheckVector[1]);
    EXPECT_EQ(mVector.get_value(2),mCheckVector[2]);

}

TEST(createAnObject, whenObjectCreated_StartAtOrigin)
{
    objectPosition currentPosition;
    std::vector<double> checkVector;
    expectEqualVectors(currentPosition.returnCreateVectorClass(),checkVector);
}

TEST(changeInXYZPosition, whenVelocityinXYZ_ExpectXYZChange)
{
    createVector currentPosition;
    createVector currentVelocity;
    std::vector<double> updateVelocity{10,10,10};
    currentVelocity.redefine_My_Vector(updateVelocity);
    double timeStep{1};
    createVector newPosition = position_of_object(currentPosition,currentVelocity,timeStep);
    std::vector<double> checkNewPosition{10,10,10};

    EXPECT_EQ(checkNewPosition[0],newPosition.get_value(0));
    EXPECT_EQ(checkNewPosition[1],newPosition.get_value(1));
    EXPECT_EQ(checkNewPosition[2],newPosition.get_value(2));
}
TEST(gravityWithoutDrag, whenNoDrag_ExpectMovement)
{

}
