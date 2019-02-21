#pragma once
#include "with_name.hpp"
#include "bindable.hpp"
#include "internal.hpp"
#include <utility>

namespace gl {
	namespace internal {
		enum buffer_target : unsigned {
			array = 0x8892,
			element_array = 0x8893,
			pixel_pack = 0x88EB,
			pixel_unpack = 0x88EC
		};

		void gen_buffers(unsigned n, unsigned* buffers);
		void bind_buffer(unsigned target, unsigned buffer);
		void delete_buffers(unsigned n, unsigned* buffers);

		void buffer_data(unsigned target, unsigned size, const void* data, unsigned usage);
		void buffer_sub_data(unsigned target, unsigned offset, unsigned size, const void* data);
		void get_buffer_parameteriv(unsigned target, unsigned pname, int* data);
	}

	enum buffer_usage : unsigned {
		static_draw = 0x88E4,
		dynamic_draw = 0x88E8
	};

	class vertex_array;

	class buffer : public with_name, protected bindable {
	protected:
		using with_name::with_name;
	public:
		buffer(buffer&& r) = default;
	};

	namespace internal {
	template<internal::buffer_target Tar>
	class buffer_impl : public buffer {
		friend vertex_array;
		template<class T>
		friend std::shared_ptr<T> view(unsigned name);
		static constexpr internal::buffer_target target = Tar;

		buffer_impl(unsigned name):buffer(name){}
	protected:
		void bind() override {
			internal::bind_buffer(target, name);
		}
	public:
		//using buffer::buffer;

		buffer_impl() {
			internal::gen_buffers(1, &name);
			bind();
		}

		buffer_impl(buffer_impl&& r) = default;

		template<class Container>
		buffer_impl(Container c) : buffer_impl() {
			data<Container>(c);
		}

		//buffer(unsigned name, internal::buffer_target tar) : with_name{ name }, target{ tar } {}

		~buffer_impl() {
			if (name != invalid_name) {
				internal::delete_buffers(1, &name);
				invalidate_name();
			}
		}

		size_t size() {
			bind();
			int size;
			internal::get_buffer_parameteriv(target, 0x8764, &size);
			return size;
		}

		template<class RAI>
		void data(RAI begin, RAI end, gl::buffer_usage usage = gl::buffer_usage::static_draw) {
			bind();
			internal::buffer_data(target, std::distance(begin, end) * sizeof(std::iterator_traits<RAI>::value_type), &*begin, usage);
		}

		template<class Container>
		std::enable_if_t<std::is_class_v<Container>>
		data(Container container, buffer_usage usage = gl::buffer_usage::static_draw) {
			bind();
			internal::buffer_data(target, sizeof(typename Container::value_type)*(unsigned)container.size(), container.data(), usage);
		}

		template<class size_t_>
		std::enable_if_t<std::is_integral_v<size_t_>>
		data(size_t_ size, buffer_usage usage) {
			bind();
			internal::buffer_data(target, size, nullptr, usage);
		}

		template<class Container>
		void sub_data(unsigned offset, Container container) {
			bind();
			internal::buffer_sub_data(target, offset, sizeof(typename Container::value_type)*(unsigned)container.size(), container.data());
		}
	};
	}

	using array_buffer = internal::buffer_impl<internal::array>;
	using element_array_buffer = internal::buffer_impl<internal::element_array>;
}