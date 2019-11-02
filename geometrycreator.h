#ifndef GEOMETRYCREATOR_H
#define GEOMETRYCREATOR_H
#include <osg/Geode>

class geometryCreator
{
public:
    geometryCreator();
    void makeMainSphere(osg::Sphere* sphere, osg::Geode* geode, float sphereRadius);
    void makeBoundaryBox(osg::Geode* mgeode, float cubeLength);
    void makeGoalBox(osg::Geode* mgeode, float cubeLength, std::vector<float> cornerPosition);
};

#endif // GEOMETRYCREATOR_H
