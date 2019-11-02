#include "stdtoosgadapt.h"
#include <osg/Vec3>
#include <vector>

osg::Vec3 convert_physics_vector_to_vec3(std::vector<double> mVector)
{
    float mX = static_cast<float>(mVector[0]);
    float mY = static_cast<float>(mVector[1]);
    float mZ = static_cast<float>(mVector[2]);
    osg::Vec3 physicsVectorAsVec3(mX,mY,mZ);

    return physicsVectorAsVec3;
}
