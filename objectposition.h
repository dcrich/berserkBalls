#ifndef OBJECTPOSITION_H
#define OBJECTPOSITION_H
#include "createvector.h"

class objectPosition
{
public:
    objectPosition();
    createVector returnCreateVectorClass();

private:
    createVector currentPosition;

};

#endif // OBJECTPOSITION_H
