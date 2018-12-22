#pragma once
#include "internal.hpp"

namespace gl {
	class context;

	namespace internal {
		void set_current_context(context* c);
		context* get_current_context();
		void init();
		void get_intergerv(unsigned pname, int*data);
	}

	class context {
		int major;
		int minor;
	public:
		context() {
			gl::internal::init();
			internal::get_intergerv(0x821B, &major);
			internal::get_intergerv(0x821C, &minor);
		}

		int get_major() const { return major; }
		int get_minor() const { return minor; }
	};

	inline void wrap_context() {
		internal::set_current_context(new gl::context());
	}
}