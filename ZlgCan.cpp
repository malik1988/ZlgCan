#include "ZlgCan.h"
#include <string>
namespace ZlgCan {
	ZlgErrorCode ZlgCan::ZlgCan::Open(ZlgCanBaudrate baud)
	{
		if (VCI_OpenDevice(ZLG_CAN_DEV) != STATUS_OK)
			return ZlgErrorCode::����_���豸ʧ��;

		int baudrate = static_cast<int>(baud);
		if (VCI_SetReference(ZLG_CAN_DEV, 0, &baudrate) != STATUS_OK)
			return ZlgErrorCode::����_���ò�����ʧ��;

		VCI_INIT_CONFIG init_config;
		memset(&init_config, 0, sizeof(init_config));
		init_config.AccMask = 0xffffff;
		if (VCI_InitCAN(ZLG_CAN_DEV, &init_config) != STATUS_OK)
			return ZlgErrorCode::����_�����˲�����ʧ��;
		if (VCI_StartCAN(ZLG_CAN_DEV) != STATUS_OK)
			return ZlgErrorCode::����_�����豸ʧ��;
		return ZlgErrorCode::�ɹ�;
	}
	ZlgErrorCode ZlgCan::Reset()
	{
		if (VCI_ResetCAN(ZLG_CAN_DEV) != STATUS_OK)
			return ZlgErrorCode::ʧ��;
		return ZlgErrorCode::�ɹ�;
	}
	ZlgErrorCode ZlgCan::Send(uint32_t canId, std::vector<char>& data, bool remote, bool externed)
	{
		if (data.empty() || data.size() > 8)
			return ZlgErrorCode::����_��Ч���ݳ���;

		VCI_CAN_OBJ frame;
		memset(&frame, 0, sizeof(frame));

		//����֡���͡� 
		//= 0ʱΪ�������ͣ�
		//= 1ʱΪ���η��ͣ����Զ��ط����� 
		//= 2ʱΪ�Է����գ����ڲ���CAN���Ƿ��𻵣��� 
		//= 3ʱΪ�����Է����գ�ֻ����һ�Σ������Բ��ԣ���ֻ�ڴ�֡Ϊ����֡ʱ�����塣

		frame.SendType = 0;

		//���ݳ���DLC(<= 8)����Data�ĳ��ȡ�
		frame.DataLen = static_cast<BYTE>(data.size());

		//�Ƿ���Զ��֡��=0ʱΪ����֡��=1ʱΪԶ��֡��
		frame.RemoteFlag = remote ? 1 : 0;

		//�Ƿ�����չ֡�� = 0ʱΪ��׼֡��11λ֡ID���� = 1ʱΪ��չ֡��29λ֡ID��
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
			return ZlgErrorCode::ʧ��;
		return ZlgErrorCode::�ɹ�;
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
			return ZlgErrorCode::ʧ��;
		}
		canId = canObj.ID;
		data.clear();
		for (int i = 0;i < canObj.DataLen;i++)
			data.push_back(canObj.Data[i]);

		return ZlgErrorCode::�ɹ�;
	}
	ZlgErrorCode ZlgCan::ReadErrInfo()
	{
		return ZlgErrorCode();
	}
	ZlgErrorCode ZlgCan::Close()
	{
		if (VCI_CloseDevice(CAN_DEV_TYPE, CAN_DEV_INDEX) != STATUS_OK)
			return ZlgErrorCode::ʧ��;
		return ZlgErrorCode::�ɹ�;
	}
}