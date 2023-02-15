#pragma once

#include <vector>
#include <memory>

#include "Shape.h"
#include "AABB.h"

// Splitting Algorithms for selecting the two children
// 1. Median Split (basic v1)
// 2. Surface Area Heuristic (advanced v2)

int medianSplit(std::vector<std::shared_ptr<Shape>> list, int size, float pivot, int axis);