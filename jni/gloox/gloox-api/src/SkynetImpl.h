#ifndef SKYNETIMPL_H__
#define SKYNETIMPL_H__
#include "ChatClient.h"
#include "../inc/LoginListener.h"
#include "../inc/SendMessageListener.h"
#include "../inc/JoinRoomListener.h"
#include "../inc/LeaveRoomListener.h"
#include "../inc/MessageReceiverListener.h"
#include "ServerConfig.h"
#include "Packet.h"
#include "../../gloox/src/client.h"
#include "../../gloox/src/message.h"
#include "../../gloox/src/messagehandler.h"
#include "../../gloox/src/connectionlistener.h"
#include "../../gloox/src/mucroomhandler.h"
#include "../../gloox/src/presence.h"
#include "../../gloox/src/mucroom.h"
#include "../../gloox/src/mutex.h"

#include <iostream>
#include <string>
#include "../inc/Skynet.h"
#include <thread>
#include "LogConfig.h"
#include "MessageExt.h"

namespace skynet 
{
	class SkynetImpl
		: public gloox::MessageHandler
		, public gloox::LogHandler
		, public gloox::ConnectionListener
		, public gloox::MUCRoomHandler
		, public gloox::PresenceHandler
	{
	public:
		static SkynetImpl* getInstance();
		static SkynetImpl* getInstance(Skynet::EnvironmentType type);
		void setServer(const std::string& server);
		void setPort(int port);
		void setUid(const std::string& uid);
		void setPassword(const std::string& password);

		const std::string& getServer() const;
		int getPort() const;

		void initialize(const std::string & appid);
		void login(const Skynet::LoginInfo& info, const LoginListener* loginListener);
		void sendMessage(const Skynet::SendMessageInfo& info,const SendMessageListener* sendMessageListener);
		void joinChatRoom(const Skynet::JoinChatRoomInfo& info, const JoinRoomListener* joinRoomListener);
		void leaveChatRoom(const Skynet::LeaveChatRoomInfo& info, const LeaveRoomListener* leaveRoomListener);
		bool logout();
		void registerMessageReceiverListener(const MessageReceiverListener* messageReceiverListener);
		void unregisterMessageReceiverListener(const MessageReceiverListener* messageReceiverListener);
		virtual void handleMessage( const gloox::Message& stanza, gloox::MessageSession* session = 0 );
		virtual void  handleLog (gloox::LogLevel level, gloox::LogArea area, const std::string &message);
		virtual void onConnect();
		virtual void onDisconnect( gloox::ConnectionError e );
		virtual bool onTLSConnect( const gloox::CertInfo& info );

		virtual void handleMUCParticipantPresence( gloox::MUCRoom* room, const gloox::MUCRoomParticipant participant,const gloox::Presence& presence );
		virtual void handleMUCMessage( gloox::MUCRoom* room, const gloox::Message& msg, bool priv );
		virtual bool handleMUCRoomCreation( gloox::MUCRoom* room );
		virtual void handleMUCSubject( gloox::MUCRoom* room, const std::string& nick,const std::string& subject );
		virtual void handleMUCInviteDecline( gloox::MUCRoom* room, const gloox::JID& invitee,const std::string& reason );
		virtual void handleMUCError( gloox::MUCRoom* room, gloox::StanzaError error );
		virtual void handleMUCInfo( gloox::MUCRoom* room, int features, const std::string& name,const gloox::DataForm* infoForm );
		virtual void handleMUCItems( gloox::MUCRoom* room, const gloox::Disco::ItemList& items );
		virtual void handlePresence( const gloox::Presence& presence );
	private:
		SkynetImpl(Skynet::EnvironmentType type);
		virtual ~SkynetImpl();
		ChatClient * mClient;
		std::string mServer;
		int mPort;
		std::string mDomain;
		std::string mUid;
		std::string mPassword;
		std::string mResource;
		std::list<const LoginListener*> mLoginListenerList;
		std::list<const MessageReceiverListener*> mMessageReceiverListenerList;
		std::list<const JoinRoomListener*> mJoinRoomListenerList;
		std::list<const LeaveRoomListener*> mLeaveRoomListenerList;
		std::map<std::string,gloox::MUCRoom*> mJoinRoomMap;
		gloox::util::Mutex mLoginListenerListMutex;
		gloox::util::Mutex mMessageReceiverListenerListMutex;
		gloox::util::Mutex mJoinRoomListenerListMutex;
		gloox::util::Mutex mLeaveRoomListenerListMutex;
		gloox::util::Mutex mJoinRoomMapMutex;
		gloox::util::Mutex mIsLoginMutex;
		gloox::util::Mutex mIsSendMessageMutex;
		static gloox::util::Mutex sInscanceMutex;
		static SkynetImpl* sInscance;
		bool mIsLogin;	
		bool mIsSend;
		Packet mPacket;
	};
}
#endif //SKYNETIMPL_H__