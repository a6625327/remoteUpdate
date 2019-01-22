#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

#define MAX_NAME_LENGTH 128

#define FRAME_HEAD 0xa1
#define FRAME_TAIL 0xa2

// 布尔类型声明
enum boolen { true = 1, false = !(true) };

typedef uint8_t *StreamPtr;

typedef struct {
  uint8_t name[MAX_NAME_LENGTH + 1];

} FileInfo;

#endif // !__COMMON_H__
