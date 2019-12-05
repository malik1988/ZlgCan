#pragma once
namespace ZlgCan {
	enum class ZlgCanBaudrate :int
	{
		B1000K = 0x060003,
		B800K = 0x060004,
		B500K = 0x060007,
		B250K = 0x1C0008,
		B125K = 0x1C0011,
		B100K = 0x160023,
		B50K = 0x1C002C,
		B20K = 0x1600B3,
		B10K = 0x1C00E0,
		B5K = 0x1C01C1
	};
}
