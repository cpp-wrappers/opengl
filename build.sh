mkdir -p build
cd build
rm *

src_files=\
'buffer '\
'core '\
'debug '\
'program '\
'shader '\
'texture '\
'vertex_array'

#for x in ${src_files}; do src_files0="../src/gl/${x}.cpp "; done
$CXX $CXXFLAGS -c -std=c++17 \
-I ../code \
`echo ${src_files} | sed 's/\w*/..\/code\/gl\/&.cpp/g'`

ar -rs libopengl-wrapper.a `echo ls | sed 's/\w*/&.o/g'`

