#pragma once
#include <vector>
#include "ZlgErrorCode.h"
#include "ControlCAN.h"
#include "ZlgCanBaudrate.h"

namespace ZlgCan {

#define ZLG_CAN_DEV CAN_DEV_TYPE,CAN_DEV_INDEX,CAN_DEV_ID

	class ZlgCan
	{
	public:
		ZlgErrorCode Open(ZlgCanBaudrate baud);
		ZlgErrorCode Reset();
		ZlgErrorCode Send(uint32_t canId, std::vector<char>& data, bool remote = false, bool externed = false);
		ZlgErrorCode Receive(uint32_t& canId, std::vector<char>& data, int timeOut = -1);
		ZlgErrorCode ReadErrInfo();
		ZlgErrorCode Close();
	private:
		//CAN 设备类型
		const int CAN_DEV_TYPE = VCI_USBCAN_E_U;
		//CAN 设备索引号
		const int CAN_DEV_INDEX = 0;
		//CAN 设备编号
		const int CAN_DEV_ID = 0;
	};

}