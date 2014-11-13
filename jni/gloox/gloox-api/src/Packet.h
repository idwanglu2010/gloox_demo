#ifndef PACKET_H__
#define PACKET_H__
#include <iostream>
#include "StringUtil.h"
namespace skynet
{
	class Packet
	{
	public:
		Packet();
		~Packet();
		std::string getPacketID();
	private:
		static long sId;
		static std::string sPrefix;
	};
}


#endif // PACKET_H__