#ifndef TETRIS_COUNTER_H
#define TETRIS_COUNTER_H
class Counter {
private:
    int maxCount;
    int currCount;
public:
    Counter(int maxCount) {
        this->maxCount = maxCount;
        this->currCount = 0;
    }

    void tick() {
        currCount++;
    }

    bool isFinished() {
        return currCount == maxCount;
    }

    void reset() {
        currCount = 0;
    }
};

#endif //TETRIS_COUNTER_H