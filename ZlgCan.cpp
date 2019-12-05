#include "ZlgCan.h"
#include <string>
namespace ZlgCan {
	ZlgErrorCode ZlgCan::ZlgCan::Open(ZlgCanBaudrate baud)
	{
		if (VCI_OpenDevice(ZLG_CAN_DEV) != STATUS_OK)
			return ZlgErrorCode::错误_打开设备失败;

		int baudrate = static_cast<int>(baud);
		if (VCI_SetReference(ZLG_CAN_DEV, 0, &baudrate) != STATUS_OK)
			return ZlgErrorCode::错误_设置波特率失败;

		VCI_INIT_CONFIG init_config;
		memset(&init_config, 0, sizeof(init_config));
		init_config.AccMask = 0xffffff;
		if (VCI_InitCAN(ZLG_CAN_DEV, &init_config) != STATUS_OK)
			return ZlgErrorCode::错误_设置滤波参数失败;
		if (VCI_StartCAN(ZLG_CAN_DEV) != STATUS_OK)
			return ZlgErrorCode::错误_启动设备失败;
		return ZlgErrorCode::成功;
	}
	ZlgErrorCode ZlgCan::Reset()
	{
		if (VCI_ResetCAN(ZLG_CAN_DEV) != STATUS_OK)
			return ZlgErrorCode::失败;
		return ZlgErrorCode::成功;
	}
	ZlgErrorCode ZlgCan::Send(uint32_t canId, std::vector<char>& data, bool remote, bool externed)
	{
		if (data.empty() || data.size() > 8)
			return ZlgErrorCode::错误_无效数据长度;

		VCI_CAN_OBJ frame;
		memset(&frame, 0, sizeof(frame));

		//发送帧类型。 
		//= 0时为正常发送，
		//= 1时为单次发送（不自动重发）， 
		//= 2时为自发自收（用于测试CAN卡是否损坏）， 
		//= 3时为单次自发自收（只发送一次，用于自测试），只在此帧为发送帧时有意义。

		frame.SendType = 0;

		//数据长度DLC(<= 8)，即Data的长度。
		frame.DataLen = static_cast<BYTE>(data.size());

		//是否是远程帧。=0时为数据帧，=1时为远程帧。
		frame.RemoteFlag = remote ? 1 : 0;

		//是否是扩展帧。 = 0时为标准帧（11位帧ID）， = 1时为扩展帧（29位帧ID）
		frame.ExternFlag = externed ? 1 : 0;

		if (frame.ExternFlag == 1)
		{
			frame.ID = canId;
		}
		else
		{
			frame.ID = canId & 0x0000ffff;
		}
		std::copy(data.begin(), data.end(), frame.Data);
		if (VCI_Transmit(ZLG_CAN_DEV, &frame, 1) != STATUS_OK)
			return ZlgErrorCode::失败;
		return ZlgErrorCode::成功;
	}
	ZlgErrorCode ZlgCan::Receive(uint32_t& canId, std::vector<char>& data, int timeOut)
	{
		int count = 0;
		VCI_CAN_OBJ canObj;
		count = VCI_Receive(ZLG_CAN_DEV, &canObj, 1, timeOut);
		if (count <= 0)
		{
			VCI_ERR_INFO err;
			VCI_ReadErrInfo(ZLG_CAN_DEV, &err);
			return ZlgErrorCode::失败;
		}
		canId = canObj.ID;
		data.clear();
		for (int i = 0;i < canObj.DataLen;i++)
			data.push_back(canObj.Data[i]);

		return ZlgErrorCode::成功;
	}
	ZlgErrorCode ZlgCan::ReadErrInfo()
	{
		return ZlgErrorCode();
	}
	ZlgErrorCode ZlgCan::Close()
	{
		if (VCI_CloseDevice(CAN_DEV_TYPE, CAN_DEV_INDEX) != STATUS_OK)
			return ZlgErrorCode::失败;
		return ZlgErrorCode::成功;
	}
}