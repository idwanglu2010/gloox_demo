#ifndef LEAVEROOMLISTENER_H__
#define LEAVEROOMLISTENER_H__

#include <string>
namespace skynet 
{
	class LeaveRoomListener
	{
	public:
		virtual ~LeaveRoomListener() {}
		virtual void onLeaveRoom(const std::string& roomId) const = 0;
	private:

	};
}
#endif //LEAVEROOMLISTENER_H__