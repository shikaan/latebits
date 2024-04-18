#pragma once

#include "core/geometry/Box.h"
#include "core/geometry/Vector.h"

using namespace lb;
using namespace std;

// Return true if positions are in the same cell
auto match(Vector p1, Vector p2) -> bool;

// Return true if boxes intersect
auto intersects(Box box1, Box box2) -> bool;

// Return true if container contains content
auto contains(Box container, Box content) -> bool;