#include "../inc/Skynet.h"
#include "SkynetImpl.h"
#include "LogConfig.h"
#include <thread>

#define TAG "Skynet"

namespace skynet
{
	Skynet::Skynet()
	{
	}

	Skynet::~Skynet()
	{
	}

	void Skynet::initialize(const InitializeInfo& info)
	{
		EnvironmentType type = info.type;
		LOGD(TAG,"initialize begin, type: %d\n",type);
		SkynetImpl *impl = SkynetImpl::getInstance(type);
		impl->initialize(info.appid);
		LOGD(TAG,"initialize end");
	}

	void Skynet::login(const LoginInfo& info, const LoginListener* loginListener)
	{
		SkynetImpl *impl = SkynetImpl::getInstance();
		//impl->login(info,loginListener);
		std::thread t(&SkynetImpl::login,impl,info,loginListener);
		t.detach();
	}

	void Skynet::sendMessage(const SendMessageInfo& info, const SendMessageListener* sendMessageListener)
	{
		SkynetImpl *impl = SkynetImpl::getInstance();
		//impl->sendMessage(info,sendMessageListener);
		std::thread t(&SkynetImpl::sendMessage,impl,info,sendMessageListener);
		t.detach();
	}

	void Skynet::joinChatRoom(const JoinChatRoomInfo& info, const JoinRoomListener* joinRoomListener)
	{
		SkynetImpl *impl = SkynetImpl::getInstance();
		//impl->joinChatRoom(info,joinRoomListener);
		std::thread t(&SkynetImpl::joinChatRoom,impl,info,joinRoomListener);
		t.detach();
	}
	void Skynet::leaveChatRoom(const LeaveChatRoomInfo& info, const LeaveRoomListener* leaveRoomListener)
	{
		SkynetImpl *impl = SkynetImpl::getInstance();
		//impl->leaveChatRoom(info,leaveRoomListener);
		std::thread t(&SkynetImpl::leaveChatRoom,impl,info,leaveRoomListener);
		t.detach();
	}
	bool Skynet::logout()
	{
		SkynetImpl *impl = SkynetImpl::getInstance();
		std::thread t(&SkynetImpl::logout,impl);
		t.detach();
		//impl->logout();
		return true;
	}
	void Skynet::registerMessageReceiverListener(const MessageReceiverListener* messageReceiverListener)
	{
		SkynetImpl *impl = SkynetImpl::getInstance();
		impl->registerMessageReceiverListener(messageReceiverListener);
	}
	void Skynet::unregisterMessageReceiverListener(const MessageReceiverListener* messageReceiverListener)
	{
		SkynetImpl *impl = SkynetImpl::getInstance();
		impl->unregisterMessageReceiverListener(messageReceiverListener);
	}
}
