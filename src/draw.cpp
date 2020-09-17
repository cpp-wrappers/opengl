#include "../include/gl/draw.hpp"

using namespace gl;

#ifdef GL_ALL_INCLUE_HEADER
    #include GL_ALL_INCLUE_HEADER
#else
    #define GL_GLEXT_PROTOTYPES
    #include "GL/gl.h"
    #include "GL/glcorearb.h"
#endif

#define _1(N,GLN,T,GLT) void gl::internal:: N (T v0) { gl##GLN##GLT (v0); }
#define _4(N,GLN,T,GLT) void gl::internal:: N (T v0, T v1, T v2, T v3) { gl##GLN##GLT (v0, v1, v2, v3); }

#define _N1(N,GLN,T,GLT) void gl::internal:: N (T v0) { gl##GLN##1##GLT (v0); }
#define _N2(N,GLN,T,GLT) void gl::internal:: N (T v0, T v1) { gl##GLN##2##GLT (v0, v1); }
#define _N3(N,GLN,T,GLT) void gl::internal:: N (T v0, T v1, T v2) { gl##GLN##3##GLT (v0, v1, v2); }
#define _N4(N,GLN,T,GLT) void gl::internal:: N (T v0, T v1, T v2, T v3) { gl##GLN##4##GLT (v0, v1, v2, v3); }

#define _N23(N,GLN,T,GLT) _N2(N,GLN,T,GLT) _N3(N,GLN,T,GLT)
#define _N34(N,GLN,T,GLT) _N3(N,GLN,T,GLT) _N4(N,GLN,T,GLT)
#define _N234(N,GLN,T,GLT) _N23(N,GLN,T,GLT) _N4(N,GLN,T,GLT)
#define _N1234(N,GLN,T,GLT) _N1(N,GLN,T,GLT) _N234(N,GLN,T,GLT)

#define SIFD(P,N,GLN) _##P(N,GLN,short,s) _##P(N,GLN,int,i) _##P(N,GLN,float,f) _##P(N,GLN,double,d)
#define BSIFD(P,N,GLN) _##P(N,GLN,char,b) SIFD(P,N,GLN)

SIFD(N234,vertex,Vertex)
SIFD(N1234,tex_coord,TexCoord)
BSIFD(N3,normal,Normal)
SIFD(1,index,Index)
SIFD(4,rect,Rect)

void gl::internal::begin(unsigned mode) { glBegin(mode); }
void gl::internal::end() { glEnd(); }