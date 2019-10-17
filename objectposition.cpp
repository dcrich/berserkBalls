#include <iostream>
#include "objectposition.h"
#include "createvector.h"

objectPosition::objectPosition()
{
}

createVector objectPosition::returnCreateVectorClass()
{
    return currentPosition;
}


