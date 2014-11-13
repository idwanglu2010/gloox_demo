#ifndef SKYNET_H__
#define SKYNET_H__
#include <string>
#include "LoginListener.h"
#include "SendMessageListener.h"
#include "JoinRoomListener.h"
#include "LeaveRoomListener.h"
#include "MessageReceiverListener.h"
namespace skynet
{
	class Skynet
	{
	public:
		enum MessageType
		{
			Chat           =  0,        /**< A chat message. */
			Groupchat      =  1,        /**< A groupchat message. */
		};

		enum EnvironmentType
		{
			Dev           =  1,
			Test		  =  2, 
			Sandbox		  =  3,
			Official	  =  4,
			Self		  =  5,
		};
		struct InitializeInfo
		{
			EnvironmentType type;
			std::string appid;
		};
		struct LoginInfo
		{
			std::string uid;
			std::string sessionid;
		};
		struct SendMessageInfo
		{
			std::string toUid;
			std::string body;
			MessageType type;
		};
		struct JoinChatRoomInfo
		{
			std::string roomid;
			std::string roomnick;
		};
		struct LeaveChatRoomInfo
		{
			std::string roomid;
			std::string roomnick;
		};

		static void initialize(const InitializeInfo& info);
		static void login(const LoginInfo& info, const LoginListener* loginListener);
		static void sendMessage(const SendMessageInfo& info, const SendMessageListener* sendMessageListener);
		static void joinChatRoom(const JoinChatRoomInfo& info, const JoinRoomListener* joinRoomListener);
		static void leaveChatRoom(const LeaveChatRoomInfo& info, const LeaveRoomListener* leaveRoomListener);
		static bool logout();
		static void registerMessageReceiverListener(const MessageReceiverListener* messageReceiverListener);
		static void unregisterMessageReceiverListener(const MessageReceiverListener* messageReceiverListener);
		
	private:
		Skynet();
		virtual ~Skynet();
	};
}
#endif	// SKYNET_H__