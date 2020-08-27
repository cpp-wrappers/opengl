#pragma once

#include <memory>
#include <limits>
#include "internal.hpp"

namespace gl {

using name_t = unsigned;

class with_name {

protected:
	static constexpr name_t invalid_name = std::numeric_limits<unsigned>::max();

	name_t name{ invalid_name };

	void invalidate_name() {
		name = invalid_name;
	}
public:
	with_name() = default;
	with_name(name_t name): name{ name } {}
	with_name(with_name&& o): name{ o.name } {
		o.invalidate_name();
	}

	with_name& operator=(with_name&& o) {
		name = o.name;
		o.invalidate_name();
		return *this;
	}
};
}