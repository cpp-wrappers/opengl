#define NT1(N,T) void N (T v0);
#define NT2(N,T) void N (T v0, T v1);
#define NT3(N,T) void N (T v0, T v1, T v2);
#define NT4(N,T) void N (T v0, T v1, T v2, T v3);
#define NT6(N,T) void N (T v0, T v1, T v2, T v3, T v4, T v5);
#define NT23(N,T) NT2(N,T) NT3(N,T)
#define NT34(N,T) NT3(N,T) NT4(N,T)
#define NT234(N,T) NT23(N,T) NT4(N,T)
#define NT1234(N,T) NT1(N,T) NT234(N,T)
#define D(NUM,N) NT##NUM(N,double)
#define FD(NUM,N) NT##NUM(N,float) D(NUM,N)
#define SIFD(NUM,N) NT##NUM(N,short) NT##NUM(N,int) FD(NUM,N)
#define BSIFD(NUM,N) NT##NUM(N,char) SIFD(NUM,N)