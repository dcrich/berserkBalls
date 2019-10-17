#ifndef CREATEVECTOR_H
#define CREATEVECTOR_H
#include <iostream>
#include <vector>

class createVector
{
public:
    createVector();
    void redefine_My_Vector(std::vector<double> theNewVector);
    double get_value(size_t i);
    std::vector<double> return_vector();
    size_t get_size();
    void set_value(size_t i, double setValue);
    void print_My_Vector();
    ~createVector();
private:
    size_t vectorSize{3};
    std::vector<double> theVector;
};
#endif // CREATEVECTOR_H
