#pragma once
#include <type_traits>
#include <cstdint>
using uint = unsigned;

namespace gl {
	class context;

	namespace internal {

		template<class T, class C=std::remove_cvref_t<T>>
		constexpr uint type_token() {
			if constexpr (std::is_same_v<C, int8_t>)
				return 0x1400;
			if constexpr (std::is_same_v<C, uint8_t>)
				return 0x1401;
			if constexpr (std::is_same_v<C, int32_t>)
				return 0x1404;
			if constexpr (std::is_same_v<C, uint32_t>)
				return 0x1405;
			if constexpr (std::is_same_v<C, float>)
				return 0x1406;
		}
	}
}