#include "sphereupdatecallback.h"
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Vec3>
#include <vector>
#include <osg/PositionAttitudeTransform>
#include "objectposition.h"
#include "stdtoosgadapt.h"
#include "worldcollisions.h"

sphereUpdateCallback::sphereUpdateCallback(objectPosition *spherePosition, float cubeLength, float sphereRadius)
{
    mPosition = spherePosition;
    mBoundary = cubeLength;
    mRadius = sphereRadius;
}

sphereUpdateCallback::~sphereUpdateCallback()
{
    delete mPosition;
}

void sphereUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    std::vector<double> coordinates {mPosition->get_position()};
    std::vector<double> velocity {mPosition->get_velocity()};
    mPosition->update_position();
    coordinates  = mPosition->get_position();
    std::vector<int> collisionCheck = check_for_collision(mPosition,mBoundary,mRadius);

    for (size_t i{0};i<collisionCheck.size();i++)
    {
        if (collisionCheck[i])
        {
            mPosition->static_collision(i);
        }
    }
    mPosition->update_position();

    osg::Vec3d positionValue {convert_physics_vector_to_vec3(coordinates)};
    osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *> (node);
    pat->setPosition(positionValue);

    traverse(node, nv);
}
