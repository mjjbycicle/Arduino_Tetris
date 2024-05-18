#pragma once

#include <stdint.h>

/// A zero-cost strong type
template <typename T, typename Parameter>
class NamedType
{
public:
	explicit NamedType(T const& value) : value_(value) {}

	NamedType() : value_() {}

	T& get() { return value_; }
	T const& get() const {return value_; }
private:
	T value_;
};

using pin = NamedType<uint8_t, struct pinTag>;