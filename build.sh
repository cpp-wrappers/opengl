if [[ -z $CXX ]]; then
    echo you must specify CXX variable;
    exit
fi

cxxargs=\
"-c \
--std=c++17 \
-I../../include"

cxxargs=$cxxargs $CXXARGS

