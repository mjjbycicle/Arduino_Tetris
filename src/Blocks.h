#include "Helpers.h"

#ifndef TETRIS_BLOCKS_H
#define TETRIS_BLOCKS_H
#define Z_COLOR 0x3B5C
#define S_COLOR 0xFFE0
#define SQ_COLOR 0x065F
#define LINE_COLOR 0x07D3
#define RL_COLOR 0xEC84
#define L_COLOR 0x881f
#define T_COLOR 0xf86e
#define NONE_COLOR 0

uint16_t getColor(char type) {
    switch (type) {
        case 'L':
            return L_COLOR;
        case 'R':
            return RL_COLOR;
        case 'S':
            return S_COLOR;
        case 'Z':
            return Z_COLOR;
        case 'T':
            return T_COLOR;
        case 'I':
            return LINE_COLOR;
        case 'Q':
            return SQ_COLOR;
        default:
            return NONE_COLOR;
    }
}

char
        L[5][5]{
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'L', ' ', ' '},
        {' ', ' ', 'L', ' ', ' '},
        {' ', ' ', 'L', 'L', ' '},
        {' ', ' ', ' ', ' ', ' '},
},
        RL[5][5]{
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'R', ' ', ' '},
        {' ', ' ', 'R', ' ', ' '},
        {' ', 'R', 'R', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
},
        S[5][5]{
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'S', 'S', ' '},
        {' ', 'S', 'S', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
},
        Z[5][5]{
        {' ', ' ', ' ', ' ', ' '},
        {' ', 'Z', 'Z', ' ', ' '},
        {' ', ' ', 'Z', 'Z', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
},
        T[5][5]{
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'T', ' ', ' '},
        {' ', 'T', 'T', 'T', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
},
        SQ[5][5]{
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'Q', 'Q', ' '},
        {' ', ' ', 'Q', 'Q', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
},
        LINE[5][5]{
        {' ', ' ', 'I', ' ', ' '},
        {' ', ' ', 'I', ' ', ' '},
        {' ', ' ', 'I', ' ', ' '},
        {' ', ' ', 'I', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
};

void getRawVec(char (&dest)[5][5], char type) {
    switch (type) {
        case 'L':
            fill(dest, L);
            return;
        case 'R':
            fill(dest, RL);
            return;
        case 'S':
            fill(dest, S);
            return;
        case 'Z':
            fill(dest, Z);
            return;
        case 'T':
            fill(dest, T);
            return;
        case 'I':
            fill(dest, LINE);
            return;
        case 'Q':
            fill(dest, SQ);
            return;
    }
}

int getXIndex(char type) {
    return 2;
}

int getYIndex(char type) {
    if (type == 'I')
        return -4; //-4
    else
        return -3; //-3
}

#endif //TETRIS_BLOCK_H