if [[ -z $CXX ]]; then
    echo you must specify CXX variable;
    exit
fi

cxxargs=\
"-c \
--std=c++17 \
-I../include"

cxxargs=$cxxargs $CXXARGS

mkdir -p build
cd build

src_files=\
'buffer '\
'core '\
'debug '\
'program '\
'shader '\
'texture '\
'vertex_array'

#for x in ${src_files}; do src_files0="../src/gl/${x}.cpp "; done
$CXX $cxxargs -c `echo ${src_files} | sed 's/\w*/..\/src\/gl\/&.cpp/g'`

ar -rs libopengl-wrapper.a `echo ${src_files} | sed 's/\w*/&.o/g'`

