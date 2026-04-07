//
// Created by Griffin Roberts on 3/31/26.
//

#include "TestPoint.h"
#include <random>

#include "../World.h"

namespace TestPoint {
    void test_point::random_move() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, World::world_map_size);
        x = distr(gen);
        y = distr(gen);
    }
}
