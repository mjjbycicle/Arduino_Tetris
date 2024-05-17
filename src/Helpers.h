#ifndef TETRIS_HELPERS_H
#define TETRIS_HELPERS_H

class Mod4 {
private:
    int currCount;
public:
    void set(int n) {
        currCount = n;
    }

    Mod4() {
        currCount = 0;
    }

    void advance() {
        currCount++;
        currCount %= 4;
    }

    int get() {
        return currCount;
    }
};

void fill(char (&dest)[5][5], char src[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

#endif //TETRIS_HELPERS_H