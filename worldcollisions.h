#ifndef WORLDCOLLISIONS_H
#define WORLDCOLLISIONS_H
#include "objectposition.h"
#include <vector>

std::vector<int> check_for_collision(objectPosition *position, float boundary,float sphereRadius);
bool check_for_goal(objectPosition *position, float boundary, float goalSize);
bool check_for_lose(objectPosition *position, bool didYouLose, float sphereRadius);

#endif // WORLDCOLLISIONS_H
