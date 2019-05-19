#pragma once

#include <memory>
#include "internal.hpp"

namespace gl {

class with_name {

protected:
	static constexpr uint invalid_name = std::numeric_limits<uint>::max();

	void invalidate_name() {
		name = invalid_name;
	}
public:
	with_name() {}
	with_name(uint name): name{ name } {}
	with_name(with_name&& o): name{ o.name } {
		o.invalidate_name();
	}

	with_name& operator=(with_name&& o) {
		name = o.name;
		o.invalidate_name();
		return *this;
	}

	uint name{ invalid_name };
};
}