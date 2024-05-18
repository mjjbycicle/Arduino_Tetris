#pragma once

#include <stdint.h>

/// A zero-cost strong type
template <typename T, typename Parameter>
class NamedType
{
public:
	constexpr explicit NamedType(T const& value) : value_(value) {}

	constexpr NamedType() : value_() {}

	T& get() { return value_; }
	constexpr T const& get() const {return value_; }

	bool operator==(const NamedType<T, Parameter>& other) {
		return value_ == other.value_;
	}
private:
	T value_;
};

using pin = NamedType<uint8_t, struct pinTag>;

using Direction = NamedType<int8_t, struct DirectionTag>;
Direction rotateCCW(Direction direction) {
	return Direction {static_cast<int8_t>((direction.get() + 1) & 3)};
}
Direction rotateCW(Direction direction) {
	return Direction {static_cast<int8_t>((direction.get() - 1) & 3)};
}
int8_t getDeltaX(Direction direction) {
	int8_t& i = direction.get();
	return static_cast<int8_t>(((i & 2) - 1) * (i & 1));
}
int8_t getDeltaY(Direction direction) {
	int8_t& i = direction.get();
	return static_cast<int8_t>(((i & 2) - 1) * (~i & 1));
}

static const Direction DIRECTION_U {0};
static const Direction DIRECTION_L {1};
static const Direction DIRECTION_D {2};
static const Direction DIRECTION_R {3};

class Cooldown {
	const int duration;
	int numTicksLeft {0};
public:
	explicit Cooldown(int duration) : duration(duration) {}

	void reset() {
		numTicksLeft = duration;
	}

	void skip() {
		numTicksLeft = 0;
	}

	void tick() {
		if(!isFinished()) numTicksLeft --;
	}

	bool isFinished() const {
		return numTicksLeft == 0;
	}
};