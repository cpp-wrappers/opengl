#pragma once
#include "with_name.hpp"
#include <string>

namespace gl {
	namespace internal {
		unsigned create_shader(unsigned type);
		void delete_shader(unsigned shader);
		void shader_source(unsigned shader, unsigned count, const char* const* string, const int* length);
		void compile_shader(unsigned shader);
		void get_shaderiv(unsigned shader, unsigned pname, int* params);
		void get_shader_info_log(unsigned shader, unsigned buf_size, int* length, char* info_log);

		enum shader_type : unsigned {
			compute_shader = 0x91B9,
			vertex_shader = 0x8B31,
			fragment_shader = 0x8B30
		};
	}

	class program;

	class shader : public with_name {
	protected:
		using with_name::with_name;
	public:
		shader(shader&) = delete;
		shader& operator=(shader&) = delete;
		shader(shader&& s) = default;

		virtual void source(std::string src) = 0;
		virtual shader& compile() = 0;
	};

	namespace internal {
	template<internal::shader_type Type>
	class shader_impl : public shader {
		friend program;
		static constexpr internal::shader_type type = Type;
	public:
		shader_impl():shader(internal::create_shader(type)) {}
		shader_impl(shader_impl&& r) = default;

		shader_impl(std::string src):shader_impl() {
			source(src);
			compile();
		}

		~shader_impl() {
			if (name != invalid_name) {
				internal::delete_shader(name);
				invalidate_name();
			}
		}

		void source(std::string src) override {
			const char* c_str = src.c_str();
			internal::shader_source(name, 1, &c_str, nullptr);
		}

		shader& compile() override {
			internal::compile_shader(name);
			int success;
			internal::get_shaderiv(name, 0x8B81, &success);
			if(!success) {
				int length;
				internal::get_shaderiv(name, 0x8B84, &length);
				char* log = new char[length];
				internal::get_shader_info_log(name, length, nullptr, log);
				std::string log_str{log};
				delete[] log;
				throw std::runtime_error(log_str);
			}
			return *this;
		}
	};
	}
	using vertex_shader = internal::shader_impl<internal::vertex_shader>;
	using fragment_shader = internal::shader_impl<internal::fragment_shader>;
}