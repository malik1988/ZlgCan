#pragma once
#include<iostream>
namespace ZlgCan {
	enum class  ZlgErrorCode :int
	{
		成功 = 0,
		失败 = 1,
		错误_打开设备失败,
		错误_设置波特率失败,
		错误_设置滤波参数失败,
		错误_启动设备失败,

		错误_无效数据长度,
	};

}
