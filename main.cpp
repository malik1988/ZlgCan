#include "ZlgCan.h"
#include <iostream>
int main(int argc,char ** argv)
{
	ZlgCan::ZlgCan can;
	auto code = can.Open(ZlgCan::ZlgCanBaudrate::B125K);
	if (code == ZlgCan::ZlgErrorCode::³É¹¦)
	{
		std::vector<char> data;
		data.push_back(0x11);
		code = can.Send(0x330, data);

		std::cout << "Can Send :" << static_cast<int>(code) << std::endl;
	}
	else
		std::cout << "Can Open Failed:" << static_cast<int>(code) << std::endl;
	return 0;
}