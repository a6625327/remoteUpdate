#include "analysis.h"

// analysisStream(Analysis_Struct analysis_str, )

// typedef struct  __EVENT_DRIVE
// {
// 	MODE_TYPE mod;//消息的发送模块
// 	EVENT_TYPE event;//消息类型
// 	STATUS_TYPE status;//自身状态
// 	EVENT_FUN eventfun;//此状态下的处理函数指针
// }EVENT_DRIVE;

// EVENT_DRIVE eventdriver[] =
// //这就是一张表的定义，不一定是数据库中的表。也可以使自己定义的一个结构体数组。
// {
// 	{MODE_A, EVENT_a, STATUS_1, fun1}
// 	{MODE_A, EVENT_a, STATUS_2, fun2}
// 	{MODE_A, EVENT_a, STATUS_3, fun3}
// 	{MODE_A, EVENT_b, STATUS_1, fun4}
// 	{MODE_A, EVENT_b, STATUS_2, fun5}

// 	{MODE_B, EVENT_a, STATUS_1, fun6}
// 	{MODE_B, EVENT_a, STATUS_2, fun7}
// 	{MODE_B, EVENT_a, STATUS_3, fun8}
// 	{MODE_B, EVENT_b, STATUS_1, fun9}
// 	{MODE_B, EVENT_b, STATUS_2, fun10}
// };

// int driversize = sizeof(eventdriver) / sizeof(EVENT_DRIVE)//驱动表的大小

// EVENT_FUN GetFunFromDriver(MODE_TYPE mod, EVENT_TYPE event, STATUS_TYPE
// status)//驱动表查找函数
// {
// 	int i = 0;
// 	for (i = 0; i < driversize; i ++)
// 	{
// 		if ((eventdriver[i].mod == mod) && (eventdriver[i].event ==
// event)
// && (eventdriver[i].status == status))
// 		{
// 			return eventdriver[i].eventfun;
// 		}
// 	}
// 	return NULL;
// }

/*
该子程序将一段流作为输入，并且分析该流的内容，从中剖离出
与相应协议相关的内容，存入至相关的结构体中
*/
void AnalysisStream(StreamPtr i_stream, const uint16_t i_streamLength,
                    FileInfo o_struct) {
    /*
       while 未到流尾部

    */
}