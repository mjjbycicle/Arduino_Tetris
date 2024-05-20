#pragma once

#include "Vec.h"
#include "util.h"

constexpr vec
        rightToDownKickData[]{
        vec(0, 0),
        vec(1, 0),
        vec(1, -1),
        vec(0, -2),
        vec(1, 2)
},
        downToLeftKickData[]{
        vec(0, 0),
        vec(1, 0),
        vec(1, 1),
        vec(0, -2),
        vec(1, -2)
},
        leftToUpKickData[]{
        vec(0, 0),
        vec(-1, 0),
        vec(-1, -1),
        vec(0, 2),
        vec(-1, 2)
},
        upToRightKickData[]{
        vec(0, 0),
        vec(-1, 0),
        vec(-1, 1),
        vec(0, -2),
        vec(-1, -2)
};
