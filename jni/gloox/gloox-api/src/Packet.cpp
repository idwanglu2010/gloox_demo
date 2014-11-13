#include "Packet.h"
namespace skynet
{
	long Packet::sId = 0L;

	std::string Packet::sPrefix(StringUtil::randomString(6).append("-"));

	Packet::Packet()
	{
	}

	Packet::~Packet()
	{
	}

	std::string Packet::getPacketID()
	{
		std::string packetId = sPrefix;
		char buffer[256];
		memset(buffer, 0, 256);
		sprintf( buffer , "%ld", sId++ );
		packetId.append(buffer);
		return packetId;
	}
}
