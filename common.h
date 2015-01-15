//this has to be outside the main sketch as library, 
//otherwise I get compilation errors
#ifndef __COMMON_H__
#define __COMMON_H__
#ifndef __BYTEBOOL__
    #define __BYTEBOOL__
    typedef enum { qfalse, qtrue } qboolean;
    typedef unsigned char byte;
#endif
#endif
