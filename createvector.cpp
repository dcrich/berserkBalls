#include <iostream>
#include <vector>
#include "createvector.h"

createVector::createVector()
{
    theVector={0,0,0};
}
void createVector::redefine_My_Vector(std::vector<double> theNewVector)
{
    theVector = theNewVector;
}
double createVector::get_value(size_t i)
{
    return theVector[i];
}
std::vector<double> createVector::return_vector()
{
    return theVector;
}
size_t createVector::get_size()
{
    return vectorSize;
}
void createVector::set_value(size_t i, double setValue)
{
    theVector[i] = setValue;
}
void createVector::print_My_Vector()
{
    std::cout<<"[ ";
    for(size_t i{0};i<theVector.size();i++)
    {
        std::cout<<theVector[i]<<" ";
    }
    std::cout<<"]"<<std::endl;
}
createVector::~createVector()
{
}






