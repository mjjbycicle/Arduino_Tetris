#ifndef TETRIS_COORDINATES_H
#define TETRIS_COORDINATES_H

class point {
private:
	int8_t x_, y_;
public:
	point () : point(0, 0) {}
	point (int8_t x, int8_t y) : x_(x), y_(y) {}

	int8_t x () const {
		return x_;
	}

	int8_t& x () {
		return x_;
	}

	int8_t y () const {
		return y_;
	}

	int8_t& y () {
		return y_;
	}

	void moveUp () {
		y_--;
	}

	void moveDown () {
		y_++;
	}

	void moveRight () {
		x_++;
	}

	void moveLeft () {
		x_--;
	}
};

#endif //TETRIS_COORDINATES_H