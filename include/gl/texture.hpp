#pragma once

#include "with_name.hpp"
#include "unified_math/vec2.hpp"
#include <type_traits>
#include <ranges>

using uint = unsigned;

namespace gl {
namespace internal {
	void gen_textures(uint n, uint* textures);
	void bind_texture(uint target, uint texture);
	void delete_textures(uint n, uint* textures);

	void texture_parameteri(uint texture, uint pname, int param);
	void tex_parameteri(uint target, uint pname, int param);
	void get_texture_level_parameteriv(uint texture, int level, uint pname, int* params);
	void get_tex_level_parameteriv(uint target, int level, uint pname, int* params);
	void tex_image_2d(uint target, int level, int internalformat,
		uint width, uint height, int border, uint format, uint type, const void* data);
	void tex_sub_image_2d(uint target, int level, int xoffset,
		int yoffset, uint width, uint height, uint format,
		uint type, const void *data);
	void active_texture(uint texture);

	enum texture_target : unsigned {
		none = 0,
		texture_2d = 0x0DE1
	};
}

namespace internal {
	template<texture_target Tar, unsigned Dim = 0>
	struct texture;
}

template<class T>
concept texture = std::is_base_of_v<internal::texture<internal::texture_target::none>, T>;

template<texture Texture>
void active_texture(Texture& tex, uint index);

enum internal_format : uint {
	rgba8 = 0x8058
};

enum pixel_format : uint {
	bgra = 0x80E1,
	rgb = 0x1907,
	rgba = 0x1908
};

enum wrap_mode : uint {
	clamp_to_border = 0x812D,
	clamp_to_edge = 0x812F,
	repeat = 0x2901
};

enum filter {
	nearest = 0x2600,
	linear
};

namespace internal {

template<>
struct texture<none> : protected with_name {
	using with_name::with_name;
};

template<texture_target Tar>
struct texture<Tar> : protected texture<none> {
protected:
	template<gl::texture Texture>
	friend void gl::active_texture(Texture& tex, uint index);
	
	static constexpr internal::texture_target target = Tar;

	void bind() {
		bind_texture(target, name);
	}

	int get_level_parameter_i(uint pname, int level = 0) {
		int result;
		bind();
		get_tex_level_parameteriv(target, level, pname, &result);
		return result;
	}

	void parameter(uint pname, int value) {
		bind();
		tex_parameteri(target, pname, value);
	}

	using texture<none>::texture;

	texture() {
		gen_textures(1, &name);
		bind();
	}

	~texture() {
		if (name != invalid_name) {
			delete_textures(1, &name);
			invalidate_name();
		}
	}
public:
	void mag_filter(filter filter) {
		parameter(0x2800, (uint)filter);
	}

	void min_filter(filter filter) {
		parameter(0x2801, (uint)filter);
	}

	void filter(filter filter) { min_filter(filter); mag_filter(filter); }
};

template<texture_target target>
struct texture<target, 2> : texture<target, 0> {
	using base = texture<target, 0>;

	using base::base;

	uint width() { return base::get_level_parameter_i(0x1000); }
	uint height() { return base::get_level_parameter_i(0x1001); }
	template<uni::vec2ui Vec = uni::pair_f>
	Vec size() { return {width(), height()}; }

	template<class T> requires std::is_arithmetic_v<T>
	void image(int level, internal_format internalformat, uni::vec2ui auto size, pixel_format format, const T* data) {
		base::bind();
		tex_image_2d(
			target, level, internalformat,
			std::get<0>(size), std::get<1>(size), 0,
			format, type_token<T>, data
		);
	}

	template<class T>
	void image(internal_format internalformat, uni::vec2ui auto size, pixel_format format, const T* data) {
		image<T>(0, internalformat, size, format, data);
	}

	template<std::ranges::random_access_range R>
	void image(int level, internal_format internalformat, uni::vec2ui auto size, pixel_format format, const R& data) {
		image<std::ranges::range_value_t<R>>(
			level, internalformat, size, format, data.data()
		);
	}

	template<std::ranges::random_access_range R>
	void image(internal_format internalformat, uni::vec2ui auto size, pixel_format format, const R& data) {
		image(0, internalformat, size, format, data);
	}

	template<class T>
	void sub_image(int xoffset, int yoffset, int width, int height, pixel_format format, const T* data) {
		base::bind();
		tex_sub_image_2d(target, 0, xoffset, yoffset, width, height, format, type_token<T>, data);
	}
};

}

struct texture_2d : internal::texture<internal::texture_target::texture_2d, 2> {
	using texture<internal::texture_target::texture_2d, 2>::texture;
};

template<texture Texture>
void active_texture(Texture& tex, uint index) {
	internal::active_texture(0x84C0 + index);
	tex.bind();
}

}