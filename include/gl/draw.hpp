#pragma once

#include "primitive.hpp"
#include "unified_math/vec.hpp"
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
    void vertex(uni::vec2f);
    void tex_coord(uni::vec2f);
    void normal(uni::vec2f);
    void fog_coord(float coord);
    void color(uni::vec4f color);
    void secondary_color(uni::vec3f color);
    void index(int);
    void vertex_attrib(uni::vec4f attrib);
}

using mode = primitive_type;
struct geometric_objects;
using geometric_objects_declarer = std::function<void(geometric_objects&)>;

void inline begin(mode m, const geometric_objects_declarer god);

namespace internal {
    struct edge_flag_specifier {};
    struct tex_coord_specifier {};
    struct normal_specifier {};
    struct fog_coord_specifier {};
    struct color_specifier {};
    struct secondary_color_specifier {};
    struct index_specifier {};
    struct attrib_specifier {};

    template<class... Ts>
    struct vertex_specifier : Ts... {

        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<edge_flag_specifier>>
        edge_flag(bool flag) requires tv::type_vector<Ts...>::template contains_v<edge_flag_specifier> {
            internal::edge_flag(flag); return {};
        }

        template<class... Ts0> requires tv::type_vector<Ts...>::template contains_v<tex_coord_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<tex_coord_specifier>>
        tex_coord(Ts0... ts) {
            internal::tex_coord(uni::vec_from(ts...)); return {};
        }

        template<class... Ts0> requires tv::type_vector<Ts...>::template contains_v<normal_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<normal_specifier>>
        normal(Ts0... ts) {
            internal::vertex(uni::vec_from(ts...)); return {};
        }

        template<class T> requires tv::type_vector<Ts...>::template contains_v<fog_coord_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<fog_coord_specifier>>
        fog_coord(T coord) {
            internal::fog_coord(coord); return {};
        }

        template<class... Ts0> requires tv::type_vector<Ts...>::template contains_v<color_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<color_specifier>>
        color(Ts0... ts) {
            internal::color(uni::vec_from(ts...)); return {};
        }

        template<class... Ts0> requires tv::type_vector<Ts...>::template contains_v<secondary_color_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<secondary_color_specifier>>
        secondary_color(Ts0... ts) {
            internal::secondary_color(uni::vec_from(ts...)); return {};
        }

        template<class T> requires tv::type_vector<Ts...>::template contains_v<index_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<index_specifier>>
        index(T ts) {
            internal::index(ts); return {};
        }

        template<class... Ts0> requires tv::type_vector<Ts...>::template contains_v<attrib_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<attrib_specifier>>
        attrib(Ts0... ts) {
            internal::vertex_attrib(uni::vec_from(ts...)); return {};
        }
    };

    template<class... Ts>
    struct vertex_specifier<tv::type_vector<Ts...>> : vertex_specifier<Ts...> {};
}

class geometric_objects {
    friend void inline begin(mode m, const geometric_objects_declarer god);
    ~geometric_objects() { internal::end(); }
public:

    template<class... Ts>
    internal::vertex_specifier<
        internal::edge_flag_specifier, internal::tex_coord_specifier,
        internal::normal_specifier, internal::fog_coord_specifier,
        internal::color_specifier, internal::secondary_color_specifier,
        internal::index_specifier, internal::attrib_specifier
    >
    vertex(Ts... ts) { internal::vertex(uni::vec_from(ts...)); return{}; }
};

void inline begin(mode m, const geometric_objects_declarer god) {
    internal::begin((unsigned)m);
    geometric_objects go;
    god(go);
}

}