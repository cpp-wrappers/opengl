#pragma once
#include <stdint.h>
#include <type_traits>
#include <cstdint>
using uint = unsigned;

namespace gl {

	namespace internal {

		template<class T>
		inline uint type_token;

		template<> inline uint type_token<int8_t> =  0x1400;
		template<> inline uint type_token<uint8_t> =  0x1401;
		template<> inline uint type_token<int16_t> =  0x1402;
		template<> inline uint type_token<uint16_t> =  0x1403;
		template<> inline uint type_token<int32_t> =  0x1404;
		template<> inline uint type_token<uint32_t> =  0x1405;
		template<> inline uint type_token<float> =  0x1406;
	}
}