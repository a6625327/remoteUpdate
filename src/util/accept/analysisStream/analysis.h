#ifndef __ANALYSIS_STREAM_H__
#define __ANALYSIS_STREAM_H__
#include "../../../../include/common.h"

typedef uint8_t Analysis_t;

typedef void (*Analysis_Func)();

typedef struct __Analysis_Struct {
    Analysis_t type;
    Analysis_Func func;
} Analysis_Struct;

#endif // !__ANALYSIS_STREAM_H__
