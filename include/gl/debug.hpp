#pragma once
#include <functional>
#include <string>

namespace gl {
	enum message_source : uint {
	};

	enum message_type : uint {
		error = 0x824C,
		deprecated_behavior,
		undefined_behavior,
		portability,
		performance,
		marker,
		push_group,
		pop_group,
		other
	};

	enum message_severity {
	};

	typedef void (debug_callback)(message_source source, message_type type, uint id,
			message_severity severity, uint length, const char *message,
			const void *user_param);

	namespace internal {
		inline void debug_message_callback(debug_callback, const void *user_param = nullptr);
	}
}

#ifdef GL_INCLUDE 
	#include "gl/debug.cpp"
#endif