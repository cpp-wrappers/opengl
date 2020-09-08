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
    void vertex(float x, float y);
    void tex_coord(float f0, float f1);
    void normal(float x, float y, float z);
    void fog_coord(float coord);
    void color(float r, float g, float b, float a);
    void secondary_color(float r, float g, float b);
    void index(int);
    void vertex_attrib(float f0, float f1, float f2, float f3);
}

using mode = primitive_type;
struct geometric_objects;
using geometric_objects_declarer = std::function<void(geometric_objects&)>;

void inline begin(mode m, const geometric_objects_declarer god);

namespace internal {
    struct edge_flag_specifier;
    struct tex_coord_specifier;
    struct normal_specifier;
    struct fog_coord_specifier;
    struct color_specifier;
    struct secondary_color_specifier;
    struct index_specifier;
    struct attrib_specifier;

    template<class... Ts>
    struct vertex_specifier;

    template<class... Ts>
    struct vertex_specifier<tv::type_vector<Ts...>> {
        //using specs = typename tv::type_vector<Ts...>;

        //template<class T>
        //static constexpr bool _contains = specs::template contains_v<T>;

        //template<class T>
        //using _erase = vertex_specifier<typename specs::template erase_first_t<T>>;

        /*erase<edge_flag_specifier> edge_flag(bool flag)
        requires contains<edge_flag_specifier> {
            internal::edge_flag(flag); return {};
        }

        template<class... Ts0> requires contains<tex_coord_specifier>
        erase<tex_coord_specifier> tex_coord(Ts0... ts) {
            internal::tex_coord(ts...); return {};
        }*/

        template<class... Ts0>
        requires tv::type_vector<Ts...>::template contains_v<normal_specifier>
        vertex_specifier<typename tv::type_vector<Ts...>::template erase_first_t<normal_specifier>>
        normal(Ts0... ts) {
            internal::normal(ts...); //return {};
        }

        /*template<class T> requires contains<fog_coord_specifier>
        erase<fog_coord_specifier> fog_coord(T coord) {
            internal::fog_coord(coord); return {};
        }

        template<class... Ts0> requires contains<color_specifier>
        erase<color_specifier> color(Ts0... ts) {
            internal::color(ts...); return {};
        }

        template<class... Ts0> requires contains<secondary_color_specifier>
        erase<secondary_color_specifier> secondary_color(Ts0... ts) {
            internal::secondary_color(ts...); return {};
        }

        template<class T> requires  contains<index_specifier>
        erase<index_specifier> index(T ts) {
            internal::index(ts); return {};
        }

        template<class... Ts0> requires contains<attrib_specifier>
        erase<attrib_specifier> attrib(Ts0... ts) {
            internal::vertex_attrib(ts...); return {};
        }*/
    };

    //template<class... Ts>
    //struct vertex_specifier<tv::type_vector<Ts...>> : vertex_specifier<Ts...> {};
}

class geometric_objects {
    friend void inline begin(mode m, const geometric_objects_declarer god);
    ~geometric_objects() { internal::end(); }
public:

    //template<class... Ts>
    //internal::vertex_specifier<
        //internal::edge_flag_specifier, internal::tex_coord_specifier,
   //     internal::normal_specifier//, internal::fog_coord_specifier,
        //internal::color_specifier, internal::secondary_color_specifier,
        //internal::index_specifier, internal::attrib_specifier
   // >
    internal::vertex_specifier<tv::type_vector<internal::normal_specifier, int>>
    vertex() { return{}; }
};

void inline begin(mode m, const geometric_objects_declarer god) {
    internal::begin((unsigned)m);
    geometric_objects go;
    god(go);
}

}