#pragma once

#include "bindable.hpp"
#include "with_name.hpp"
#include "buffer.hpp"
#include "internal.hpp"
#include "primitive.hpp"

namespace gl {
	namespace internal {
		inline void gen_vertex_arrays(uint n, uint* arrays);
		inline void bind_vertex_array(uint array);
		inline void delete_vertex_arrays(uint n, uint* arrays);

		inline void vertex_attrib_pointer(uint index, int size, uint type, bool normalized, uint stride, const void *pointer);
		inline void vertex_attrib_i_pointer(uint index, int size, uint type, uint stride, const void *pointer);
		inline void bind_vertex_buffer(uint bindingindex, uint buffer, uint offset, uint stride);
		inline void enable_vertex_attrib_array(uint index);
		inline void get_vertex_attribiv(uint index, uint pname, int *params);
	}

	namespace vertex_attribute {
		typedef uint size;
		typedef uint location;
		typedef bool normalized;
	}

	class program;

	class vertex_array : public bindable, with_name {
		friend class program;

		void bind() override { internal::bind_vertex_array(name); };
	public:
		~vertex_array() {
			if (name != invalid_name) {
				internal::delete_vertex_arrays(1, &name);
				invalidate_name();
			}
		}

		vertex_array() {
			internal::gen_vertex_arrays(1, &name);
			bind();
		}

		vertex_array(vertex_array&& v) = default;

		vertex_array(uint name) :with_name{ name } {}

		/*std::shared_ptr<array_buffer> attrib_array_buffer(uint index) {
			bind();
			uint buffer;
			internal::get_vertex_attribiv(index, 0x889F, (int*)&buffer);
			return internal::view<array_buffer>(buffer);
		}*/

		template<class T, int size>
		void attrib_pointer(vertex_attribute::location location, array_buffer& buff, vertex_attribute::normalized normalized = false) {
			bind();
			buff.bind();
			internal::vertex_attrib_pointer(location, size, internal::type_token<T>(), normalized, 0, nullptr);
		}

		template<class T, int size>
		void attrib_i_pointer(vertex_attribute::location location, array_buffer& buff) {
			bind();
			buff.bind();
			internal::vertex_attrib_i_pointer(location, size, internal::type_token<T>(), 0, nullptr);
		}

		uint attrib_size(uint index) {
			bind();
			uint size;
			internal::get_vertex_attribiv(index, 0x8623, (int*)&size);
			return size;
		}

		void bind_vertex_buffer(uint binding_index, buffer& buffer) {
			bind();
			internal::bind_vertex_buffer(binding_index, ((with_name&)buffer).name, 0, 0);
		}

		void enable_attrib_array(uint index) {
			bind();
			internal::enable_vertex_attrib_array(index);
		}
	};
}