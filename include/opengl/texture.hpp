#pragma once
#include "bindable.hpp"
#include "with_name.hpp"

namespace gl {
	namespace internal {
		void gen_textures(unsigned n, unsigned* textures);
		void bind_texture(unsigned target, unsigned texture);
		void delete_textures(unsigned n, unsigned* textures);

		void texture_parameteri(unsigned texture, unsigned pname, int param);
		void tex_parameteri(unsigned target, unsigned pname, int param);
		void get_texture_level_parameteriv(unsigned texture, int level, unsigned pname, int* params);
		void get_tex_level_parameteriv(unsigned target, int level, unsigned pname, int* params);
		void tex_image_2d(unsigned target, int level, int internalformat,
			unsigned width, unsigned height, int border, unsigned format, unsigned type, const void* data);
		void tex_sub_image_2d(unsigned target, int level, int xoffset,
			int yoffset, unsigned width, unsigned height, unsigned format,
			unsigned type, const void *data);
		void active_texture(unsigned texture);

		enum texture_target :unsigned {
			texture_2d = 0x0DE1
		};
	}

	class texture;
	void active_texture(texture& tex, unsigned index);

	enum internal_format :unsigned {
		rgba8 = 0x8058
	};

	enum pixel_format :unsigned {
		rgba = 0x1908
	};

	enum wrap_mode :unsigned {
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

	class texture : public bindable, protected with_name {
		friend void active_texture(texture& tex, unsigned index);
	public:
		using with_name::with_name;
	};

	namespace internal {
	template<internal::texture_target Tar, unsigned Dim>
	class texture_impl : public texture {
	protected:
		static constexpr internal::texture_target target = Tar;

		void bind() override {
			internal::bind_texture(target, name);
		}

		int get_level_parameter_i(unsigned pname, int level = 0) {
			int result;
			bind();
			internal::get_tex_level_parameteriv(target, level, pname, &result);
			return result;
		}

		void parameter(unsigned pname, int value) {
			bind();
			gl::internal::tex_parameteri(target, pname, value);
		}
	public:
		using texture::texture;
		texture_impl(texture_impl&& tex) : texture{ std::move(tex) } {}

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

		unsigned width() {
			return get_level_parameter_i(0x1000);
		}

		std::enable_if_t<Dim >= 2, unsigned> height() {
			return get_level_parameter_i(0x1001);
		}

		void mag_filter(mag_filter filter) {
			parameter(0x2800, (unsigned)filter);
		}

		void min_filter(min_filter filter) {
			parameter(0x2801, (unsigned)filter);
		}

		template<class T>
		std::enable_if_t<Dim == 2> image(internal_format internalformat, int level, unsigned width, unsigned height, pixel_format format, T* data) {
			bind();
			gl::internal::tex_image_2d(target, level, internalformat, width, height, 0, format, internal::type_token<T>(), data);
		}

		template<class Container>
		std::enable_if_t<Dim == 2> image(internal_format internalformat, unsigned w, unsigned h, pixel_format format, Container& data) {
			image<typename Container::value_type>(internalformat, 0, w, h, format, &*data.begin());
		}

		template<class T>
		std::enable_if_t<Dim == 2> sub_image(int xoffset, int yoffset, int width, int height, pixel_format format, T* data) {
			bind();
			gl::internal::tex_sub_image_2d(target, 0, xoffset, yoffset, width, height, format, internal::type_token<T>(), data);
		}
	};
	}

	class texture_2d : public internal::texture_impl<internal::texture_target::texture_2d, 2> {
	public:
		using internal::texture_impl<internal::texture_target::texture_2d, 2>::texture_impl;
	
		template<class T>
		inline T size() {
			return T{width(), height()};
	}
	};
	//using texture_2d = internal::texture_impl<internal::texture_target::texture_2d, 2>;
	//#define texture_2d gl::internal::texture_impl<gl::internal::texture_target::texture_2d, 2>

	inline void active_texture(texture& tex, unsigned index) {
		tex.bind();
		internal::active_texture(0x84C0 + index);
	}
}