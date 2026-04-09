//
// Created by Griffin Roberts on 3/31/26.
//

#include "TestPoint.h"

#include <iostream>
#include <random>

#include "../World.h"
#include "../../config.h"

namespace TestPoint {
    void test_point::random_move() {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> distr(0, World::world_map_size * WORLD_CHUNK_SIZE - 1);
        x = distr(gen);
        y = distr(gen);
        std::cout << x << "," << y << std::endl;
    }
}
