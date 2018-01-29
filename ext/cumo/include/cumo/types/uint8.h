typedef u_int8_t dtype;
typedef u_int8_t rtype;
#define cT  cumo_cUInt8
#define cRT cT

#define m_num_to_data(x) ((dtype)NUM2UINT(x))
#define m_data_to_num(x) UINT2NUM((unsigned int)(x))
#define m_extract(x)     UINT2NUM((unsigned int)*(dtype*)(x))
#define m_sprintf(s,x)   sprintf(s,"%u",(unsigned int)(x))

#ifndef UINT8_MIN
#define UINT8_MIN (0)
#endif

#ifndef UINT8_MAX
#define UINT8_MAX (255)
#endif

#define DATA_MIN UINT8_MIN
#define DATA_MAX UINT8_MAX

#define M_MIN  INT2FIX(0)
#define M_MAX  m_data_to_num(UINT8_MAX)

#include "uint_macro.h"
