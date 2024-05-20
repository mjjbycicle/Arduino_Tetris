#pragma once

#include <stdint.h>

/// A zero-cost strong type
template <typename T, typename Parameter>
class NamedType {
public:
	constexpr explicit NamedType (T const& value) : value_(value) {}

	constexpr NamedType () : value_() {}

	T& get () { return value_; }

	constexpr T const& get () const { return value_; }

	bool operator== (const NamedType<T, Parameter>& other) {
		return value_ == other.value_;
	}

private:
	T value_;
};

using pin = NamedType<uint8_t, struct pinTag>;

using Rotation = NamedType<int8_t, struct DirectionTag>;

Rotation rotateCCW (Rotation direction) {
	return Rotation { static_cast<int8_t>((direction.get() + 1) & 3) };
}

Rotation rotateCW (Rotation direction) {
	return Rotation { static_cast<int8_t>((direction.get() - 1) & 3) };
}

Rotation operator+ (Rotation a, Rotation b) {
	return Rotation { static_cast<int8_t>((a.get() + b.get()) & 3) };
}

Rotation getInverse (Rotation direction) {
	return Rotation { static_cast<int8_t>((4 - direction.get()) & 3) };
}

vec getOffset (Rotation direction) {
	int8_t& i = direction.get();
	return { ((i & 2) - 1) * (i & 1), ((i & 2) - 1) * (~i & 1) };
}

static constexpr Rotation DIRECTION_U { 0 };
static constexpr Rotation DIRECTION_L { 1 };
static constexpr Rotation DIRECTION_D { 2 };
static constexpr Rotation DIRECTION_R { 3 };
static constexpr Rotation ROTATE_NONE { 0 };
static constexpr Rotation ROTATE_90_CCW { 1 };
static constexpr Rotation ROTATE_180 { 2 };
static constexpr Rotation ROTATE_90_CW { 3 };

class Cooldown {
	const int duration;
	int numTicksLeft { 0 };
public:
	explicit Cooldown (int duration) : duration(duration) {}

	void reset () {
		numTicksLeft = duration;
	}

	void skip () {
		numTicksLeft = 0;
	}

	void tick () {
		if (!isFinished()) numTicksLeft--;
	}

	bool isFinished () const {
		return numTicksLeft == 0;
	}
};

template<typename T, typename U>
struct pair {
	T first;
	U second;
};