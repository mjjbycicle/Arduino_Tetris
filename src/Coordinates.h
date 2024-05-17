#ifndef TETRIS_COORDINATES_H
#define TETRIS_COORDINATES_H

class Coordinate {
private:
    int x, y;
public:
    void set(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Coordinate() {
        this->x = 0;
        this->y = 0;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getBoardX() {
        return y;
    }

    int getBoardY() {
        return 15 - x;
    }

    void moveUp() {
        y--;
    }

    void moveDown() {
        y++;
    }

    void moveRight() {
        x++;
    }

    void moveLeft() {
        x--;
    }
};

#endif //TETRIS_COORDINATES_H