#pragma once
#include "with_name.hpp"
#include "primitive.hpp"
#include "bindable.hpp"
#include "vertex_array.hpp"
#include <string>
#include "shader.hpp"

namespace internal {
template<
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

template<class T>
struct vector {
	struct element {
		using type =
			typename std::remove_reference<
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
};

}

namespace gl {
	class shader;
	class program;

	namespace internal {
		extern void bind_vertex_array(uint array);
		
		inline uint create_program();
		inline void delete_program(uint name);

		inline void attach_shader(uint program, uint shader);
		inline void detach_shader(uint program, uint shader);
		inline void link_program(uint program);
		inline void use_program(uint program);
		inline int get_uniform_location(uint program, const char *name);
		inline int get_attribute_location(uint program, const char *name);
		inline void draw_arrays(uint mode, int first, uint count);
		inline void get_program_info_log(uint program, int buf_size, int *length, char *info_log);
		inline void get_programiv(uint program, uint pname, int *params );

		inline void uniform(int location, int value);
		inline void uniform(int location, int v1, int v2);
		inline void uniform(int location, uint value);
		inline void uniform(int location, float f1);
		inline void uniform(int location, float f1, float f2);
		inline void uniform(int location, float f1, float f2, float f3, float f4);
		inline void uniform_1iv(int location, uint count, const int* value);
		inline void uniform_matrix_4fv(int location, uint count, bool transpose, const float* value);
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

		uint attrib_location(std::string attrib_name) const {
			return internal::get_attribute_location(name, attrib_name.c_str());
		}

		uint uniform_location(std::string unifrom_name) const {
			return internal::get_uniform_location(name, unifrom_name.c_str());
		}

		uint u_loc(std::string uniform_name) const {
			return uniform_location(uniform_name);
		}

		template<class T, uint N, class... Ts>
		std::enable_if_t<
			::internal::they_are_arithmetic<T, Ts...>::value
			&&
			sizeof...(Ts) == N
		>
		uniform(uint location, Ts... ts) {
			use();
			internal::uniform(location, ((T)ts)...);
		}

		template<class... Ts>
		std::enable_if_t<
			::internal::they_are_same<Ts...>::value
			&&
			std::is_arithmetic_v<typename ::internal::first<Ts...>::type>
		>
		uniform(uint location, Ts... ts) {
			uniform<
				typename ::internal::first<Ts...>::type,
				sizeof...(Ts),
				Ts...
			>(location, ts...);
		}

		template<class T, int N, class T0>
		std::enable_if_t<
			std::is_same_v<
				T,
				std::remove_reference_t<decltype(T0{}[0])>
			>
			//&&
			//::internal::vector<T0>::size::value == N
		>
		uniform(uint location, T0 value) {
			T* data = (T*)&value;

			if constexpr (N == 1)uniform<T, N>(location, data[0]);
			if constexpr (N == 2)uniform<T, N>(location, data[0], data[1]);
			if constexpr (N == 3)uniform<T, N>(location, data[0], data[2], data[3]);
			if constexpr (N == 4)uniform<T, N>(location, data[0], data[1], data[2], data[3]);
		}

		template<class T, class T0>
		std::enable_if_t<
			std::is_same_v<
				T,
				std::remove_reference_t<decltype(T0{}[0])>
			>
		>
		uniform(uint location, T0 value) {
			uniform<T, ::internal::vector<T0>::size::value, T0>(location, value);
		}

		template<class T0>
		std::enable_if_t<
			std::is_arithmetic_v<
				std::remove_reference_t<decltype(T0{}[0])>
				//typename ::internal::vector<T0>::element::type
			>
		>
		uniform(uint location, T0 value) {
			uniform<
				typename ::internal::vector<T0>::element::type,
				T0
			>(location, value);
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
		std::enable_if_t<
			std::is_same_v<
				T,
				typename ::internal::matrix<T0>::element::type
			>
			//&&
			//::internal::vector<T0>::size::value == C
		>
		uniform(uint location, uint count, bool transpose, const T0* value) const {
			//static_assert(C*R * sizeof(T) == sizeof(T2));
			use();

			if constexpr (C == 4) {
				if constexpr (R == 4) {
					if constexpr (std::is_same_v<T, float>)
					internal::uniform_matrix_4fv(location, count, transpose, (T*)value);
					else throw std::exception();
				}
				else throw std::exception();
			}
			else throw std::exception();
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
		void uniform(std::string u_name, T0... ts) {
			uniform<T, C, T0...>(u_loc(u_name), ts...);
		}

		template<class... T0>
		void uniform(std::string u_name, T0... ts) {
			uniform(u_loc(u_name), ts...);
		}

		template<class T, int C, int R, class... T0>
		void uniform(std::string u_name, T0... ts) {
			uniform<T, C, R, T0...>(u_loc(u_name), ts...);
		}

	};
}

#ifdef GL_INCLUDE 
	#include "gl/program.cpp"
#endif