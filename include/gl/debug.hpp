#pragma once

#include <functional>
#include <string>

namespace gl {

	enum message_source : unsigned {
	};

	enum message_type : unsigned {
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

	struct debug_message {
		std::string_view what;
	};

	using debug_message_callback_t = std::function<void(debug_message)>;
	void debug_message_callback(debug_message_callback_t callback);
}