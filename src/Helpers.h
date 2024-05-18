#pragma once

class RotationAmount {
private:
    int currCount;
public:
    void set(int n) {
        currCount = n;
    }

    RotationAmount() {
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