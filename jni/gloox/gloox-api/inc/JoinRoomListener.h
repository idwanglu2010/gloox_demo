#ifndef JOINROOMLISTENER_H__
#define JOINROOMLISTENER_H__
#include <string>
namespace skynet
{
	class JoinRoomListener
	{
	public:
		virtual ~JoinRoomListener() {}
		virtual void onJoinRoom(const std::string& roomId,int count) const = 0;
		virtual void onJoinFailed(const std::string& roomId,const std::string reason) const = 0;
	private:

	};
}
#endif // JOINROOMLISTENER_H__