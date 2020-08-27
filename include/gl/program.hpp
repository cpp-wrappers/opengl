#pragma once
#include "with_name.hpp"
#include "primitive.hpp"
#include "vertex_array.hpp"
#include <string>
#include <stdexcept>
#include <type_traits>
#include "shader.hpp"

namespace internal {
/*template<
	uint S,
	uint V = S,
	bool is =V*V==S
>
struct root_of_square {
	static const uint value = root_of_square<S, V-1>::value;
};

template<uint S, uint V>
struct root_of_square<S, V, true> {
	static const uint value = V;
};

template<uint S, bool is>
struct root_of_square<S, 0, is> {};

template<class T, class... Ts>
struct first {
	using type = T;
};

template<class T1, class... Ts>
struct they_are_same
: std::bool_constant<
	std::is_same<T1, typename ::internal::first<Ts...>::type>::value
	&&
	they_are_same<Ts...>::value
>{};

template<class T1>
struct they_are_same<T1> : std::bool_constant<true>{};

template<class T1, class... Ts>
struct they_are_arithmetic
: std::bool_constant<
	std::is_arithmetic_v<T1>
	&&
	they_are_arithmetic<Ts...>::value
>{};

template<class T1>
struct they_are_arithmetic<T1> : std::bool_constant<std::is_arithmetic_v<T1>>{};
*/
/*template<class T>
concept element_subs = std::is_arithmetic_v<std::remove_cvref_t<T>>;

template<class T>
concept vector = requires(T t) {
	{ t[0] } -> element_subs;
};

template<class T>
concept matrix = requires(T t) {
	{ t[0][0] } -> element_subs;
};

/*struct vector {
	struct element {
		using type =
			typename std::remove_cvref<
				decltype(T{}[0])
			>::type;

		struct size
		: std::integral_constant<uint, sizeof(type)>{};
	};

	struct size
	: std::integral_constant<uint, sizeof(T) / element::size::value>{};
};

template<class T>
struct matrix {
	struct element {
		using type =
			typename std::remove_reference<
				decltype(T{}[0][0])
			>::type;

		struct size
		: std::integral_constant<uint, sizeof(type)>{};
	};

	struct size
	: std::integral_constant<uint, sizeof(T) / element::size::value>{};
};*/

}

namespace gl {
	class shader;
	class program;

	namespace internal {
		extern void bind_vertex_array(uint array);
		
		uint create_program();
		void delete_program(uint name);

		void attach_shader(uint program, uint shader);
		void detach_shader(uint program, uint shader);
		void link_program(uint program);
		void use_program(uint program);
		int get_uniform_location(uint program, const char *name);
		int get_attribute_location(uint program, const char *name);
		void draw_arrays(uint mode, int first, uint count);
		void get_program_info_log(uint program, int buf_size, int *length, char *info_log);
		void get_programiv(uint program, uint pname, int *params );

		void uniform(int location, int value);
		void uniform(int location, int v1, int v2);
		void uniform(int location, uint value);
		void uniform(int location, float f1);
		void uniform(int location, float f1, float f2);
		void uniform(int location, float f1, float f2, float f3, float f4);
		void uniform_1iv(int location, uint count, const int* value);
		void uniform_matrix_4fv(int location, uint count, bool transpose, const float* value);
	}

	class program : public with_name {
	protected:
		static void draw_arrays_(primitive_type pt, uint start, size_t count);
	public:
		program():with_name(gl::internal::create_program()) {}
		program(program&& p):with_name{ std::move(p) } { }
		program(const program& p) = delete;

		~program() {
			if (name != invalid_name) {
				internal::delete_program(name);
				invalidate_name();
			}
		}

		template<class... Shaders>
		program(const Shaders&... shs):program() {
			attach<Shaders...>(shs...);//  see below
			link();
			detach(shs...);
		}

		template<class Shader, class... Shaders>
		std::enable_if_t<sizeof...(Shaders) >= 1>
		attach(const Shader& sh, const Shaders&... shaders) {
			attach(sh);
			attach(shaders...);
		}

		void attach(const gl::shader& sh) {
			internal::attach_shader(name, sh.name);
		}

		template<class Shader, class... Shaders>
		std::enable_if_t<sizeof...(Shaders) >= 1>
		detach(const Shader& sh, const Shaders&... shaders) {
			detach(sh);
			detach(shaders...);
		}

		void detach(const gl::shader& sh) {
			internal::detach_shader(name, sh.name);
		}

		void link() const {
			internal::link_program(name);
			int success;
			internal::get_programiv(name, 0x8B82, &success);
			if(!success) {
				int length;
				internal::get_programiv(name, 0x8B84, &length);
				char* log = new char[length];
				internal::get_program_info_log(name, length, nullptr, log);
				std::string log_str{log};
				delete[] log;
				throw std::runtime_error(log_str);
			}
		}

		void use() const {
			internal::use_program(name);
		}

		void draw_arrays(primitive_type pt, uint start, uint count) const {
			use();
			internal::draw_arrays(pt, start, count);
		}

		void draw_arrays(primitive_type pt, uint start, uint count, vertex_array& vao) const {
			use();
			internal::bind_vertex_array(vao.name);
			internal::draw_arrays(pt, start, count);
		}

		uint attrib_location(const std::string& attrib_name) const {
			return internal::get_attribute_location(name, attrib_name.c_str());
		}

		uint uniform_location(const std::string& unifrom_name) const {
			return internal::get_uniform_location(name, unifrom_name.c_str());
		}

		uint u_loc(const std::string& uniform_name) const {
			return uniform_location(uniform_name);
		}

		void uniform_matrix_4fv(unsigned loc, const float* data) {
			use();
			internal::uniform_matrix_4fv(loc, 1, false, data);
		}

		void uniform_1u(unsigned loc, unsigned val) {
			use();
			internal::uniform(loc, val);
		}

		/*template<class T0, class... Ts>
		requires (std::is_same_v<Ts, T0> && ...)
		void uniform(uint location, T0 t0, Ts... ts) {
			use();
			internal::uniform(location, t0, ts...);
		}

		template<class T>
		void uniform(uint location, T value[]) {
			if constexpr (sizeof(decltype(value)) == 1) uniform(location, value[0]);
			if constexpr (sizeof(T) == 1) uniform(location, value[0], value[1]);
			if constexpr (sizeof(T) == 1) uniform(location, value[0], value[2], value[3]);
			if constexpr (sizeof(T) == 1) uniform(location, value[0], value[1], value[2], value[3]);
		}

		void uniform(uint location, ::internal::element_subs auto value) {
			uniform(location, value);
		}

		// Second type
		template<class T, int N, class T0>
		void uniform(uint location, uint count, const T0* data) const {
			use();

			if constexpr(N == 1) {
				if constexpr (std::is_same_v<T, int>)internal::uniform_1iv(location, count, data);
				else throw std::exception();
			}
			else throw std::exception();
		}

		template<class T, class T0>
		void uniform(uint location, uint count, const T0* data) const {
			uniform<T, sizeof(T0) / sizeof(T)>(location, count, data);
		}

		template<class T0>
		void uniform(uint location, uint count, const T0* data) const {
			uniform<decltype(data[0]), T0>(location, count, data);
		}

		// Third type
		template<class T, int C, int R, class T0>
		requires(C==4 && R==4)
		std::enable_if_t<
			std::is_same_v<
				T,
				typename ::internal::matrix<T0>::element::type
			>
			//&&
			//::internal::vector<T0>::size::value == C
		>
		uniform(uint location, uint count, bool transpose, const T0* value) const {
			use();
			internal::uniform_matrix_4fv(location, count, transpose, (T*)value);
		}

		template<class T, uint C, uint R, class T0>
		std::enable_if_t<
			std::is_same_v<
				T,
				typename ::internal::matrix<T0>::element::type
			>
		>
		uniform(uint location, T0 value) const {
			uniform<T, C, R, T0>(location, 1, false, &value);
		}

		template<uint C, uint R, class T0>
		std::enable_if_t<
			std::is_arithmetic_v<
				typename std::remove_reference_t<decltype(T0{}[0][0])>
			>
		>
		uniform(uint location, T0 value) const {
			uniform<typename ::internal::matrix<T0>::element::type, C, R, T0>
			(location, value);
		}

		template<class Mat>
		std::enable_if_t<
			std::is_arithmetic_v<
				std::remove_reference_t<decltype(Mat{}[0][0])>
			>
			&&
			(
				::internal::vector<Mat>::size::value
				==
				sizeof(Mat)
				/
				sizeof(typename ::internal::vector<Mat>::element::type)
			)

		>
		uniform(uint location, Mat value) {
			uniform<
				::internal::vector<Mat>::size::value,
				::internal::vector<Mat>::size::value,
				Mat
			>
			(location, value);
		}

		// Named

		template<class T, int C, class... T0>
		void uniform(const std::string& u_name, T0... ts) {
			uniform<T, C, T0...>(u_loc(u_name), ts...);
		}

		template<class... T0>
		void uniform(const std::string& u_name, T0... ts) {
			uniform(u_loc(u_name), ts...);
		}

		template<class T, int C, int R, class... T0>
		void uniform(const std::string& u_name, T0... ts) {
			uniform<T, C, R, T0...>(u_loc(u_name), ts...);
		}*/

	};
}