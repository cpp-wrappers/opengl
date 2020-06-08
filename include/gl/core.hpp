#pragma once

#include <type_traits>

using uint = unsigned;

namespace gl {
	namespace internal {
		void clear(uint mask);
		void clear_color(float r, float g, float b, float a);
		void viewport(int x, int y, uint w, uint h);
		void enable(uint name);
		void blend_func(uint source, uint destination);
	}

	void clear_color(float r, float g, float b, float a) {
		internal::clear_color(r, g, b, a);
	}

	enum class clear_buffer : uint {
		color = 0x00004000,
		depth = 0x00000100
	};

	template<class... Mask>
	void clear(Mask... masks) {
		static_assert((std::is_same_v<gl::clear_buffer, Mask> && ...));
		internal::clear((uint)(masks | ...));
	}

	void viewport(int x, int y, uint w, uint h) {
		internal::viewport(x, y, w, h);
	}

	void enable_blending() {
		internal::enable(0x0BE2);
	}

	enum blending_factor {
		src_alpha = 0x0302,
		one_minus_src_alpha
	};

	void blend_func(blending_factor src, blending_factor dest) {
		internal::blend_func(src, dest);
	}
}