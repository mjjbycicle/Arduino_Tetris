#pragma once

#include "Vec.h"
#include "util.h"

constexpr vec
        rightToDownKickData[]{
        vec(1, 0),
        vec(1, 1),
        vec(0, 2),
        vec(1, -2)
},
        downToLeftKickData[]{
        vec(1, 0),
        vec(1, -1),
        vec(0, 2),
        vec(1, 2)
},
        leftToUpKickData[]{
        vec(-1, 0),
        vec(-1, 1),
        vec(0, -2),
        vec(-1, -2)
},
        upToRightKickData[]{
        vec(-1, 0),
        vec(-1, -1),
        vec(0, 2),
        vec(-1, 2)
};
