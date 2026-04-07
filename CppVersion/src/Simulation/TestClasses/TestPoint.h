//
// Created by Griffin Roberts on 3/31/26.
//

#ifndef C__VERSION_TESTPOINT_H
#define C__VERSION_TESTPOINT_H

namespace TestPoint {
    struct test_point {
        float x,y;
        bool moving;
        int id;
        float targetX, targetY;

        void random_move();
    };
} // TestPoint

#endif //C__VERSION_TESTPOINT_H