#pragma once

#include "with_name.hpp"

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

	enum texture_target :uint {
		texture_2d = 0x0DE1
	};
}

namespace internal {
	template<internal::texture_target Tar, uint Dim>
	class texture_impl;
}

template<internal::texture_target Tar, uint Dim>
void active_texture(internal::texture_impl<Tar, Dim>& tex, uint index);

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

enum class mag_filter {
	nearest = 0x2600,
	linear
};

enum class min_filter {
	nearest = 0x2600,
	linear
};

class texture : protected with_name {
	//friend void active_texture(texture& tex, uint index);
public:
	using with_name::with_name;
};

namespace internal {

template<internal::texture_target Tar, uint Dim>
class texture_impl : public texture {
	friend void gl::active_texture <> (texture_impl<Tar, Dim>& tex, uint index);
protected:
	static constexpr internal::texture_target target = Tar;

	void bind() {
		internal::bind_texture(target, name);
	}

	int get_level_parameter_i(uint pname, int level = 0) {
		int result;
		bind();
		internal::get_tex_level_parameteriv(target, level, pname, &result);
		return result;
	}

	void parameter(uint pname, int value) {
		bind();
		gl::internal::tex_parameteri(target, pname, value);
	}
public:
	using texture::texture;

	texture_impl(texture_impl&&) = default;
	texture_impl& operator=(texture_impl&&) = default;

	texture_impl() {
		internal::gen_textures(1, &name);
		bind();
	}

	~texture_impl() {
		if (name != invalid_name) {
			internal::delete_textures(1, &name);
			invalidate_name();
		}
	}

	uint width() {
		return get_level_parameter_i(0x1000);
	}

	std::enable_if_t<Dim >= 2, uint>
	height() {
		return get_level_parameter_i(0x1001);
	}

	void mag_filter(mag_filter filter) {
		parameter(0x2800, (uint)filter);
	}

	void min_filter(min_filter filter) {
		parameter(0x2801, (uint)filter);
	}

	template<class T>
	std::enable_if_t<Dim == 2>
	image(int level, internal_format internalformat, uint width, uint height, pixel_format format, const T* data) {
		bind();
		gl::internal::tex_image_2d(
			target,
			level,
			internalformat,
			width,
			height, 
			0,
			format,
			internal::type_token<T>(),
			data
		);
	}

	template<class T>
	std::enable_if_t<Dim == 2>
	image(internal_format internalformat, uint width, uint height, pixel_format format, const T* data) {
		image(0, internalformat, width, height, format, data);
	}

	template<class Container>
	std::enable_if_t<Dim == 2 && !std::is_pointer_v<Container>>
	image(int level, internal_format internalformat, uint w, uint h, pixel_format format, const Container& data) {
		image<typename Container::value_type>(
			level,
			internalformat,
			w,
			h,
			format,
			data.data()
		);
	}

	template<class Container>
	std::enable_if_t<Dim == 2 && !std::is_pointer_v<Container>>
	image(internal_format internalformat, uint w, uint h, pixel_format format, const Container& data) {
		image<Container>(0, internalformat, w, h, format, data);
	}

	template<class T>
	std::enable_if_t<Dim == 2>
	sub_image(int xoffset, int yoffset, int width, int height, pixel_format format, const T* data) {
		bind();
		gl::internal::tex_sub_image_2d(target, 0, xoffset, yoffset, width, height, format, internal::type_token<T>(), data);
	}
};

}

class texture_2d : public internal::texture_impl<internal::texture_target::texture_2d, 2> {
public:
	using internal::texture_impl<internal::texture_target::texture_2d, 2>::texture_impl;

	template<class Dim2>
		Dim2 size() {return Dim2{width(), height()};
	}
};

template<internal::texture_target Tar, uint Dim>
void active_texture(internal::texture_impl<Tar, Dim>& tex, uint index) {
	internal::active_texture(0x84C0 + index);
	tex.bind();
	//internal::bind_texture(Tex::target, tex.name);
}

}