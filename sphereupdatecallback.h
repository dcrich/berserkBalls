#ifndef SPHEREUPDATECALLBACK_H
#define SPHEREUPDATECALLBACK_H
#include <osg/Node>
#include <osg/NodeVisitor>
#include "objectposition.h"

class sphereUpdateCallback: public osg::NodeCallback
{
public:
    sphereUpdateCallback(objectPosition *spherePosition, float cubeLength, float sphereRadius);
    ~sphereUpdateCallback();
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
    objectPosition *mPosition;
    float mBoundary;
    float mRadius;
};

#endif // SPHEREUPDATECALLBACK_H
