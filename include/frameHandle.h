#ifndef __FRAMEHANDLE_H__
#define __FRAMEHANDLE_H__

#include <sys/socket.h>

// c stdlib
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#include "common.h"
#include "buffer.h"
#include "userLog.h"
#include "lockMgr.h"
#include "escape.h"
#include "rw.h"
#include "ringQueueStruct.h"

/*****************************************************************************
    结构体描述：代表一个帧的结构体
    head: 帧头 F1H
    frame_series_num: 流水号 数据帧序列号
    terminal_no: 终端编号 通信机编号
    type: 类别引导字 解析本数据帧的含义
    data_len: 数据体的长度
    data: 数据体
    crc: crc16校验
    tail: 帧尾 F2H
*****************************************************************************/
typedef struct{
    uint8_t  head;              // 帧头 F1H
    uint8_t  frame_series_num;  // 流水号 数据帧序列号
    uint16_t terminal_no;       // 终端编号 通信机编号
    uint8_t  type;              // 类别引导字 解析本数据帧的含义
    uint16_t data_len;          // 数据体的长度
    uint8_t  *data;             // 数据体
    uint16_t crc;               // crc16校验
    uint8_t  tail;              // 帧尾 F2H
}__attribute__((__packed__))frame_t ;

/*****************************************************************************
    结构体描述：用于储存从网络收到的流，并且用作解析用的结构体
    head_flag: 网络流中有头部，置1,反之为0
    tail_flag: 网络流中有尾部，置1,反之为0
    net_buff: 网络流缓存指针
    head_position: 网络流数据缓存中头部在网络流中的相对位置，单位字节
    tail_position: 网络流数据缓存中尾部在网络流中的相对位置，单位字节
    search_position: 作为下次查找网络流时开始查找时，在网络流数据缓存中的相对位置
*****************************************************************************/
typedef struct{
    // frame_t *frame;
    uint8_t head_flag;          // 网络流中有头部，置1,反之为0
    uint8_t tail_flag;          // 网络流中有尾部，置1,反之为0
    buff_t *net_buff;           // 网络流缓存指针
    uint16_t head_position;     // 网络流数据缓存中头部在网络流中的相对位置，单位字节
    uint16_t tail_position;     // 网络流数据缓存中尾部在网络流中的相对位置，单位字节
    uint16_t search_position;   // 作为下次查找网络流时开始查找时，在网络流数据缓存中的相对位置
}net_frame_buff_t;

/*****************************************************************************
    结构体描述：用于给帧获取序列号
    lock: 多线程安全锁
    series_num: 存储序列号，供帧使用
*****************************************************************************/
typedef struct{
    pthread_mutex_t lock;       // 多线程安全锁
    uint8_t series_num;         // 存储序列号，供帧使用
}series_num_t_p;

/*****************************************************************************
    结构体描述：用于接收数据的线程与处理数据线程的信息交互
    client_addr: 储存请求客户端的地址信息
    buf: 储存接收线程所接收的数据，供处理线程使用
*****************************************************************************/
typedef struct{
    char *client_addr;
    buff_t *buf;
}info_between_thread;

/*****************************************************************************
 函数描述  :  发送帧时需要帧序列号，每发送一次调用一次该函数，能获取到序列号
 输入参数  :  
             f: frame_t *，需要被赋值序列号的帧
 返回值    :  无 
*****************************************************************************/
void get_series_num(frame_t *f);

/*****************************************************************************
 函数描述  :  初始化一帧
 输入参数  :  
             f: frame_t *，需要被初始化的帧
             buf: void *，帧中数据部分
             buf_len: size_t，帧中数据部分长度
 返回值    :  无 
*****************************************************************************/
void init_frame(frame_t *f, void *buf, size_t buf_len);

/*****************************************************************************
 函数描述  :  向一个套接字发送一帧
 输入参数  :  
             st: int，套接字
             f: frame_t *，需要被发送的帧
 返回值    :  
              0：成功
             -1：申请内存内存失败
             -2：数据转义失败
             -3：写入到socket时失败
*****************************************************************************/
int8_t send_frame(int st, frame_t *f);

// int8_t recv_frame(int st, frame_t *f);

/*****************************************************************************
 函数描述  :  获取一个帧的大小，单位为字节，！！！包括帧数据部分！！！
 输入参数  :  
             f: frame_t *，需要被发送的帧
 返回值    :  
             帧大小
*****************************************************************************/
size_t get_frame_size(frame_t *f);

/*****************************************************************************
 函数描述  :  初始化一个接收包含帧的流的解析结构体
 输入参数  :  
             无
 返回值    :  
             解析结构体指针
*****************************************************************************/
net_frame_buff_t * init_net_frmae_buf();

/*****************************************************************************
 函数描述  :  将缓存增加到网络流解析结构体中
 输入参数  :  
             f_buf: net_frame_buff_t *，储存数据的网络流解析结构体指针
             data: void *，缓存数据指针
             data_len: size_t，缓存数据大小
 返回值    :  
              0：增加成功
             -1：增加失败
*****************************************************************************/
int8_t add_net_frame_buf(net_frame_buff_t *f_buf, void *data, size_t data_len);

/*****************************************************************************
 函数描述  :  测试一个网络流解析结构体中是否包含有一个完整的帧
 输入参数  :  
             f_buf: net_frame_buff_t *，储存数据的网络流解析结构体指针
 返回值    :  
              0：能够解析出一个完整的帧
             -1：包含的帧有头无尾
             -2：包含的帧无头有尾
             -3：检测达到了解析结构体中数据缓存尾部
             -4：包含的帧无头无尾
*****************************************************************************/
int8_t test_net_frame_buff(net_frame_buff_t *f_buf);

/*****************************************************************************
 函数描述  :  测试一个网络流解析结构体中是否包含有一个完整的帧
 输入参数  :  
             f_buf: net_frame_buff_t *，储存数据的网络流解析结构体指针
 返回值    :  
              0：能够解析出一个完整的帧
             -1：包含的帧有头无尾
             -2：包含的帧无头有尾
             -3：检测达到了解析结构体中数据缓存尾部
             -4：包含的帧无头无尾
*****************************************************************************/
int8_t add_and_test_net_frame_buff(net_frame_buff_t *f_buf, void *data, size_t data_len);

/*****************************************************************************
 函数描述  :  目前只在测试接收帧的测试程序中使用，该函数从socket接收数据并且测试是否
              包含有数据帧
 输入参数  :  
             f_buf: net_frame_buff_t *，储存数据的网络流解析结构体指针
 返回值    :  
              0：能够解析出一个完整的帧
              其他：失败
*****************************************************************************/
int8_t recv_from_socket_and_test_a_frame(struct socket_info *s_in, ring_queue_with_sem *queue);

/*****************************************************************************
 函数描述  :  将一个包含头部尾部（f1 f2）的缓存转换为一个数据帧
 输入参数  :  
             buf: void *，需要被转换的缓存
             buf_len: size_t，缓存数据长度，单位字节
             f: frame_t *，转换出来的帧将会储存在这个帧指针中
 返回值    :  
              0：成功
             -1：失败
*****************************************************************************/
int8_t switch_buff2frame_struct(void *buf, size_t buf_len, frame_t *f);

/*****************************************************************************
 函数描述  :  释放网络流解析结构体中所有资源
 输入参数  :  
             f_buf: net_frame_buff_t *，需要被转换的缓存
 返回值    :  
              无
*****************************************************************************/
void free_net_frame_buff(net_frame_buff_t *f_buf);

/*****************************************************************************
 函数描述  :  计算一个帧的crc校验码
 输入参数  :  
             t: frame_t，帧
 返回值    :  
             1：申请内存失败
             其他：crc校验码
*****************************************************************************/
uint16_t calculate_frame_crc(frame_t t);

/*****************************************************************************
 函数描述  :  获取byte_addr相对于base_addr地址的相对位置，单位为字节
 输入参数  :  
             base_addr: uint8_t *，基址
             byte_addr: uint8_t *，需要计算相对位置的地址
 返回值    :  
             相对位置值
*****************************************************************************/
uint16_t get_relative_position(uint8_t *base_addr, uint8_t *byte_addr);

#endif // !__FRAMEHANDLE_H__