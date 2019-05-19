#pragma once
#include <type_traits>

namespace gl {
	class context;

	namespace internal {

		template<class T>
		constexpr uint type_token() {
			if constexpr (std::is_same_v<T, int8_t>)
				return 0x1400;
			if constexpr (std::is_same_v<T, uint8_t>)
				return 0x1401;
			if constexpr (std::is_same_v<T, int32_t>)
				return 0x1404;
			if constexpr (std::is_same_v<T, uint32_t>)
				return 0x1405;
			if constexpr (std::is_same_v<T, float>)
				return 0x1406;
		}
	}
}