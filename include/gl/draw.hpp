#pragma once

#include "primitive.hpp"
#include "unified_math/vec2.hpp"
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include "tv/type_vector.hpp"

namespace gl {
namespace internal {
    
    void begin(unsigned mode);
    void end();
    void edge_flag(bool flag);
    void matrix_mode(unsigned mode);
    void load_matrix(float matrix[16]);
    void load_matrix(double matrix[16]);
    void load_identity();
    void push_matrix();
    void pop_matrix();

#include "internal/overload.hpp"

SIFD(234,vertex)
SIFD(1234,tex_coord)
BSIFD(3,normal)
BSIFD(34,color)
SIFD(1,index)
SIFD(4,rect)

FD(4,rotate)
FD(3,translate)
FD(3,scale)
D(6,frustum)
D(6,ortho)

#include "internal/overload_undef.hpp"
}

namespace internal {

    struct vertex_specifier {
        vertex_specifier edge_flag(bool flag) {
            internal::edge_flag(flag); return {};
        }

        template<class... Ts>
        vertex_specifier vertex(Ts... ts) {
            internal::vertex(ts...); return {};
        }
        
        template<class... Ts>
        vertex_specifier tex_coord(Ts... ts) {
            internal::tex_coord(ts...); return {};
        }

        template<class... Ts>
        vertex_specifier normal(Ts... ts) {
            internal::normal(ts...); return {};
        }

        /*template<class T>
        vertex_specifier fog_coord(T coord) {
            internal::fog_coord(coord); return {};
        }*/

        template<class... Ts> 
        vertex_specifier color(Ts... ts) {
            internal::color(ts...); return {};
        }

        /*template<class... Ts>
        vertex_specifier secondary_color(Ts... ts) {
            internal::secondary_color(ts...); return {};
        }*/

        template<class T>
        vertex_specifier index(T ts) {
            internal::index(ts); return {};
        }

        /*template<class... Ts>
        vertex_specifier attrib(Ts... ts) {
            internal::vertex_attrib(ts...); return {};
        }*/
    };
}

using primitives_declarer = internal::vertex_specifier;

//void inline begin(mode m, const geometric_objects_declarer god);

/*class draw {
    friend void inline begin(mode m, const geometric_objects_declarer god);
    ~draw() { internal::end(); }
public:
    internal::vertex_specifier
    vertex() { return{}; }
};*/

void inline begin(primitive_type m, const std::function<void(primitives_declarer)> god) {
    internal::begin((unsigned)m);
    god(internal::vertex_specifier{});
    internal::end();
}

template<class T>
void rect(uni::vec2<T> auto v0, uni::vec2<T> auto v1) {
    internal::rect(std::get<0>(v0), std::get<1>(v0), std::get<0>(v1), std::get<1>(v1));
}

enum class mode {
    texture, modelview, projection 
};

void inline matrix(mode m) {
    internal::matrix_mode((unsigned)m);
}

/*void inline load_matrix() {
    internal::matrix_mode((unsigned)m);
}*///TODO

void inline load_identity() {
    internal::load_identity();
}

}