
#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "string.h"

#ifdef DEBUG_ENABE

  // 截取文件路径，只保留文件名，__FILE__获取文件路径，strrchr从右截取'\', 获取文件名
   // 日志打印封装，__FILE__获取文件路径，__LINE__第几行，  ##表示可变参数可传可不传，__VA_ARGS__可变参数
  // #define Com_Debug_print(format, ...) printf("[%s:%d]" format, __FILE__, __LINE__, ##__VA_ARGS__)

  #define FILE_NAME strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\')+1 : __FILE__
  #define Com_Debug_print(format, ...) printf("[%s:%d]" format, FILE_NAME, __LINE__, ##__VA_ARGS__)
  #define Com_Debug_println(format, ...) printf("[%s:%d]" format "\n", FILE_NAME, __LINE__, ##__VA_ARGS__)
#else
  #define Com_Debug_print(format, ...)
  #define Com_Debug_println(format, ...)

#endif // DEBUG_ENABE


#endif /* __COM_DEBUG_H__ */
