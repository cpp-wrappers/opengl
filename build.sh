if [[ -z $CXX ]]; then
    echo you must specify CXX variable;
    exit
fi

cxxargs=\
"-c \
--std=c++17 \
-I../../include"

cxxargs=$cxxargs $CXXARGS

mkdir -p build
$CXX $cxxargs -c \
../src/gl/buffer.cpp \
../src/gl/core.cpp \
../src/gl/debug.cpp \
../src/gl/program.cpp \
../src/gl/shader.cpp \
../src/gl/texture.cpp \
../src/gl/vertex_array.cpp

ar \
buffer.o \
core.o \
debug.o \
program.o \
shader.o \
texture.o \
vertex_array.o

