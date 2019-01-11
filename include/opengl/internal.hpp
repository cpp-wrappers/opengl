#pragma once
#include <memory>
#include "context.hpp"

namespace gl {
	class context;

	namespace internal {

		
		void set_current_context(context* c);
		context* get_current_context();

		template<class T>
		std::shared_ptr<T> view(unsigned name) {
			return {
				new T(name),
				[](T* p) {p->invalidate_name();  delete p; }
			};
		}

		template<class T>
		std::shared_ptr<T> view(T& t) {
			return view<T>(t.name);
		}

		template<class T>
		static unsigned constexpr type_token() {
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
			else std::exception();
		}
	}

	const inline context* current_context() {
		return internal::get_current_context();
	}
}