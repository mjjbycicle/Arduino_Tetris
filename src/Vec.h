#pragma once

class vec {
private:
	int8_t x_, y_;
public:
	constexpr vec () : vec(0, 0) {}

	constexpr vec (int8_t x, int8_t y) : x_(x), y_(y) {}

	constexpr vec (int x, int y) : x_(static_cast<int8_t>(x)), y_(static_cast<int8_t>(y)) {}

	constexpr int8_t x () const {
		return x_;
	}

	int8_t& x () {
		return x_;
	}

	constexpr int8_t y () const {
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

	vec& operator+= (const vec& other) {
		x_ += other.x_; // NOLINT(*-narrowing-conversions)
		y_ += other.y_; // NOLINT(*-narrowing-conversions)
		return *this;
	}

	constexpr vec operator+ (const vec& other) const {
		return { x_ + other.x_, y_ + other.y_ };
	}
};