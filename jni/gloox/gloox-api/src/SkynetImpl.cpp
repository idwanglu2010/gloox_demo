#include "SkynetImpl.h"
#define TAG "SkynetImpl"
namespace skynet
{
	SkynetImpl* SkynetImpl::sInscance = NULL;
	gloox::util::Mutex SkynetImpl::sInscanceMutex;

	SkynetImpl::SkynetImpl(Skynet::EnvironmentType type) 
	{
		mIsLogin = false;
		mIsSend = true;
		ServerConfig config;
		if(type == Skynet::EnvironmentType::Dev)
		{
			ServerConfig temp(ServerConfig::Environment::Dev);
			config = temp;
		}else if(type == Skynet::EnvironmentType::Test)
		{
			ServerConfig temp(ServerConfig::Environment::Test);
			config = temp;
		}else if(type == Skynet::EnvironmentType::Sandbox)
		{
			ServerConfig temp(ServerConfig::Environment::Sandbox);
			config = temp;
		}else if(type == Skynet::EnvironmentType::Self)
		{
			ServerConfig temp(ServerConfig::Environment::Self);
			config = temp;
		}
		else 
		{
			ServerConfig temp(ServerConfig::Environment::Official);
			config = temp;
		}

		mServer = config.getServer();
		mDomain = config.getDomain();
		mPort = config.getPort();
		mResource = config.getResource();
		mPassword = config.getPassword();
		std::string str = "@" + mDomain + "/" + mResource;
		//std::string str = "admin@nick-pc/jw";
		gloox::JID jid(str);
		mClient = new ChatClient( jid, mPassword);
		mClient->registerMessageHandler( this );
		mClient->logInstance().registerLogHandler( gloox::LogLevelDebug, gloox::LogAreaAll, this );
		mClient->registerPresenceHandler( this );
		mClient->registerConnectionListener(this);
	}

	SkynetImpl::~SkynetImpl() 
	{
		mLoginListenerList.clear();
		mMessageReceiverListenerList.clear();
		mJoinRoomListenerList.clear();
		mLeaveRoomListenerList.clear();
		std::map<std::string,gloox::MUCRoom*>::iterator iter = mJoinRoomMap.begin();
		for(; iter != mJoinRoomMap.end(); iter ++)
		{
			gloox::MUCRoom* room = (*iter).second;
			delete room;
		}
		mJoinRoomMap.clear();
		delete mClient;
		mClient = NULL;
	}

	SkynetImpl* SkynetImpl::getInstance() 
	{
		return sInscance;
	}

	SkynetImpl* SkynetImpl::getInstance(Skynet::EnvironmentType type)
	{
		if(NULL == sInscance)
		{
			sInscanceMutex.lock();
			if(NULL == sInscance)
			{
				sInscance = new SkynetImpl(type);
			}
			sInscanceMutex.unlock();
		}
		return sInscance;
	}

	void SkynetImpl::setServer(const std::string& server)
	{
		mClient->setServer(server);
	}
	void SkynetImpl::setPort(int port)
	{
		mClient->setPort(port);
	}

	void SkynetImpl::setPassword(const std::string& password)
	{
		mPassword = password;
		mClient->setPassword(password);
	}

	void SkynetImpl::setUid(const std::string& uid)
	{
		mUid = uid;
		mClient->setUsername(uid);
	}

	const std::string& SkynetImpl::getServer() const
	{
		return mServer;
	}

	int SkynetImpl::getPort() const
	{
		return mPort;
	}

	void SkynetImpl::initialize(const std::string & appid)
	{

		mClient->setServer(mServer);
		mClient->setPort(mPort);
	}

	void SkynetImpl::login(const Skynet::LoginInfo& info, const LoginListener* loginListener)
	{	
		std::string sessionid = info.sessionid;
		std::string uid = info.uid;
		LOGD(TAG,"login sessionid: %s, loginListener: %p\n", sessionid.c_str(), loginListener);
		setUid(uid);
		if(mIsLogin)
		{
			LOGD(TAG,"user %s has logined!\n", mUid.c_str());
			std::list<const LoginListener*>::iterator it = mLoginListenerList.begin();
			for(; it != mLoginListenerList.end(); it++)
			{
				if(NULL != (*it))
				{
					(*it)->onLogin(mUid,true);
				}
			}
			return;
		}
		bool flag = false;
		std::list<const LoginListener*>::iterator it = mLoginListenerList.begin();
		for(; it != mLoginListenerList.end(); it++)
		{
			if(loginListener == (*it))
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			LOGD(TAG, "listener not exist in cache List, just add listener to cache List\n");
			mLoginListenerListMutex.lock();
			mLoginListenerList.push_back(loginListener);
			mLoginListenerListMutex.unlock();
		}else
		{
			LOGD(TAG, "listener exist in cache List, just ignored!\n");
		}
		LOGD(TAG, "login block connect begin\n");
		flag = mClient->connect(true);
		if(flag == false)
		{
			LOGE(TAG, "login block connect failed\n");
		}
		LOGD(TAG, "login block connect end\n");
	}

	void SkynetImpl::sendMessage(const Skynet::SendMessageInfo& info,const SendMessageListener* sendMessageListener)
	{
		mIsSendMessageMutex.lock();
		std::string toUid = info.toUid;
		std::string text = info.body;
		Skynet::MessageType type = info.type;
		LOGD(TAG,"Send Message begin\n");
		//std::string str = toUid + "@cofhistory.johnny-pc";
		std::string str;
		gloox::Message::MessageType msgType = gloox::Message::MessageType::Chat;
		if(type == Skynet::MessageType::Chat)
		{
			LOGD(TAG,"Send Chat Message, toUid: %s\n",toUid.c_str());
			msgType = gloox::Message::MessageType::Chat;
			//str = toUid + "@johnny-pc/gloox";
			str = toUid + "@" + mDomain;
			mClient->setResource(mResource);
		}else if(type == Skynet::MessageType::Groupchat)
		{
			LOGD(TAG,"Send GroupChat Message, toUid: %s\n",toUid.c_str());
			msgType = gloox::Message::MessageType::Groupchat;
			//not include resource, or will not send success
			//str = toUid + "@cofhistory.johnny-pc";
			str = toUid;
			mClient->setResource(mUid);
		}else 
		{
			LOGE(TAG,"NOT Support Operation\n");
			return;
		}

		// Send Message failed will call onDisconnect, ConnectionError == ConnIoError,so we modified mIsSend in onDisconnect function
		mIsSend = true;
		LOGD(TAG,"Send Message, toJid: %s\n",str.c_str());
		gloox::JID toJid(str);
		gloox::Message msg(msgType,toJid,text,"","","");
		std::string msgId = mPacket.getPacketID();
		LOGD(TAG,"Send Message, msgId: %s\n",msgId.c_str());
		msg.setID(msgId);
		mClient->send(msg);

		if(NULL != sendMessageListener) 
		{
			sendMessageListener->onSend(mIsSend,toUid,msgId);
		}
		LOGD(TAG,"Send Message end\n");
		mIsSendMessageMutex.unlock();
	}

	void SkynetImpl::joinChatRoom(const Skynet::JoinChatRoomInfo& info, const JoinRoomListener* joinRoomListener)
	{
		std::string roomid = info.roomid;
		std::string roomNick = info.roomnick;
		roomNick = mUid;
		LOGD(TAG,"joinChatRoom begin, roomid: %s, joinRoomListener: %p\n", roomid.c_str(), joinRoomListener);
		bool flag = false;
		std::list<const JoinRoomListener*>::iterator it = mJoinRoomListenerList.begin();
		for(;it != mJoinRoomListenerList.end(); it++)
		{
			if(joinRoomListener == (*it))
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			mJoinRoomListenerListMutex.lock();
			mJoinRoomListenerList.push_back(joinRoomListener);
			mJoinRoomListenerListMutex.unlock();
		}
		std::map<std::string,gloox::MUCRoom*>::iterator iter = mJoinRoomMap.begin();
		flag = false;
		for(;iter != mJoinRoomMap.end(); iter++)
		{
			if((*iter).first.compare(roomid) == 0)
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			LOGD(TAG, "listener not exist in cache Map, just add listener to cache Map\n");
			// example nick.wang is necessary
			//		std::string str ="1@cofhistory.johnny-pc/nick.wang";
			// roomid: 1@cofhistory.johnny-pc
			// roomNick: nick.wang
			std::string str = roomid + "/" + roomNick;
			gloox::JID toRoomJid(str);
			gloox::MUCRoom* room = new gloox::MUCRoom(mClient,toRoomJid,this,0);
			mJoinRoomMapMutex.lock();
			mJoinRoomMap.insert(std::pair<std::string,gloox::MUCRoom*>(roomid, room));
			mJoinRoomMapMutex.unlock();
			room->join();
		}else
		{
			LOGD(TAG, "listener exist in cache Map, just ignored!\n");
		}
		LOGD(TAG,"joinChatRoom end\n");
	}

	void SkynetImpl::leaveChatRoom(const Skynet::LeaveChatRoomInfo& info, const LeaveRoomListener* leaveRoomListener)
	{
		std::string roomid = info.roomid;
		std::string roomNick = info.roomnick;
		roomNick = mUid;
		LOGD(TAG,"leaveChatRoom begin, roomid: %s, leaveRoomListener: %p\n", roomid.c_str(), leaveRoomListener);
		bool flag = false;
		std::list<const LeaveRoomListener*>::iterator it = mLeaveRoomListenerList.begin();
		for(;it != mLeaveRoomListenerList.end(); it++)
		{
			if(leaveRoomListener == (*it))
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			LOGD(TAG, "listener not exist in cache List, just add listener to cache List\n");
			mLeaveRoomListenerListMutex.lock();
			mLeaveRoomListenerList.push_back(leaveRoomListener);
			mLeaveRoomListenerListMutex.unlock();
		}else
		{
			LOGD(TAG, "listener exist in cache List, just ignored!\n");
		}
		gloox::MUCRoom* roomTemp = NULL;
		std::map<std::string,gloox::MUCRoom*>::iterator iter = mJoinRoomMap.begin();
		flag = false;
		for(;iter != mJoinRoomMap.end(); iter++)
		{
			if((*iter).first.compare(roomid) == 0)
			{
				flag = true;
				roomTemp = (*iter).second;
				break;
			}
		}
		if(flag && NULL != roomTemp)
		{
			roomTemp->leave();
			LOGD(TAG,"leaveChatRoom room->leave()\n");
			delete roomTemp;
			roomTemp = NULL;
			LOGD(TAG, "find listener from cache Map success, just erase it, in order to join the room next time!\n");
			mJoinRoomMapMutex.lock();
			mJoinRoomMap.erase(iter);
			mJoinRoomMapMutex.unlock();	
			std::list<const LeaveRoomListener*>::iterator it = mLeaveRoomListenerList.begin();
			for(; it != mLeaveRoomListenerList.end(); it++)
			{
				if(NULL != (*it))
				{
					(*it)->onLeaveRoom(roomid);
				}
			}
		}
		LOGD(TAG,"leaveChatRoom end\n");
	}
	bool SkynetImpl::logout()
	{
		LOGD(TAG,"logout begin\n");
		mClient->disconnect();
		LOGD(TAG,"logout end\n");
		return true;
	}

	void SkynetImpl::registerMessageReceiverListener(const MessageReceiverListener* messageReceiverListener)
	{
		LOGD(TAG,"registerMessageReceiverListener begin, messageReceiverListener: %p\n", messageReceiverListener);
		bool flag = false;
		std::list<const MessageReceiverListener*>::iterator it = mMessageReceiverListenerList.begin();
		for(;it != mMessageReceiverListenerList.end(); it++)
		{
			if(messageReceiverListener == (*it))
			{
				flag = true;
				break;
			}
		}
		if(!flag) {
			LOGD(TAG, "listener not exist in cache List, just add listener to cache List\n");
			mMessageReceiverListenerListMutex.lock();
			mMessageReceiverListenerList.push_back(messageReceiverListener);
			mMessageReceiverListenerListMutex.unlock();
		}else
		{
			LOGD(TAG, "listener exist in cache List, just ignored!\n");
		}
		LOGD(TAG,"registerMessageReceiverListener end\n");
	}
	void SkynetImpl::unregisterMessageReceiverListener(const MessageReceiverListener* messageReceiverListener)
	{
		LOGD(TAG,"unregisterMessageReceiverListener begin, messageReceiverListener: %p\n", messageReceiverListener);
		mMessageReceiverListenerListMutex.lock();
		mMessageReceiverListenerList.remove(messageReceiverListener);
		mMessageReceiverListenerListMutex.unlock();
		LOGD(TAG,"unregisterMessageReceiverListener end\n");
	}

	void SkynetImpl::handleMessage( const gloox::Message& stanza, gloox::MessageSession* session)
	{
		LOGD(TAG,"handleMessage begin\n");
		gloox::Message::MessageType msgType = stanza.subtype();
		LOGD(TAG,"handleMessage msgType: %d\n", msgType);
		if(msgType == gloox::Message::MessageType::Chat || msgType == gloox::Message::MessageType::Normal)
		{
			gloox::JID from = stanza.from();
			std::string fromUid = from.username();
			std::string bare_from = from.bare();
			std::string full_from = from.full();
			gloox::JID to = stanza.to();
			std::string toUid = to.username();
			std::string bare_to = to.bare();
			std::string full_to = to.full();
			LOGD(TAG,"handleMessage, MessageType : Chat, from: %s, to: %s\n",fromUid.c_str(), toUid.c_str());
			if(strcmp(fromUid.c_str(),toUid.c_str()) == 0)
			{
				// self send to self, do nothing
				LOGD(TAG,"handleMessage, self send to self, just ignored!\n");
				return;
			}
			std::list<const MessageReceiverListener*>::iterator it = mMessageReceiverListenerList.begin();
			for(;it != mMessageReceiverListenerList.end(); it++)
			{
				if(NULL != (*it))
				{
					std::string body = stanza.body();
					LOGI(TAG,"handleMessage, body: %s\n", body.c_str());
					skynet::MessageReceiverListener::MessageType type = skynet::MessageReceiverListener::MessageType::Chat;
					skynet::MessageReceiverListener::Message msg;
					msg.from = fromUid;
					msg.bare_from = bare_from;
					msg.full_from = full_from;
					msg.to = toUid;
					msg.bare_to = bare_to;
					msg.full_to = full_to;
					msg.body = body;
					msg.type = type;
					std::string id = stanza.id();
					msg.timestamp = MessageExt::getInstance()->timestamp(id);
					// just remove it
					MessageExt::getInstance()->remove(id);
					std::string msgId = mPacket.getPacketID();
					msg.id = msgId;
					(*it)->onReceive(msg);
				}
			}
		}else if(msgType == gloox::Message::MessageType::Error)
		{
			// send message failed
			LOGD(TAG,"handleMessage msgType: %d, Error message\n",msgType);
		}else if(msgType == gloox::Message::MessageType::Headline)
		{
			LOGD(TAG,"handleMessage msgType: %d, Headline message\n",msgType);
		}else if(msgType == gloox::Message::MessageType::Groupchat)
		{
			LOGD(TAG,"handleMessage msgType: %d, Groupchat message\n",msgType);
		}else if(msgType == gloox::Message::MessageType::Invalid)
		{
			LOGD(TAG,"handleMessage msgType: %d, Invalid message\n",msgType);
		}
		else
		{
			// Never executed here
			LOGD(TAG,"handleMessage Never executed here\n");
		}
		LOGD(TAG,"handleMessage end\n");
	}

	void  SkynetImpl::handleLog (gloox::LogLevel level, gloox::LogArea area, const std::string &message)
	{
		//std::cout<<"handleLog:"<<message<<"\n";
		LOGI(TAG,"%s\n",message.c_str());
	}


	void SkynetImpl::onConnect()
	{
		LOGD(TAG,"onConnect\n");
		mIsLoginMutex.lock();
		mIsLogin = true;
		mIsLoginMutex.unlock();
		std::list<const LoginListener*>::iterator it = mLoginListenerList.begin();
		for(; it != mLoginListenerList.end(); it++)
		{
			if(NULL != (*it))
			{
				(*it)->onLogin(mUid,true);
			}
		}
	}

	void SkynetImpl::onDisconnect( gloox::ConnectionError e ) 
	{ 
		LOGW(TAG,"onDisconnect: disconnected: %d\n", e );
		//AuthenticationError
		mIsLoginMutex.lock();
		mIsLogin = false;
		mIsLoginMutex.unlock(); 
		if(e == gloox::ConnectionError::ConnAuthenticationFailed)
		{
			// login failed
			LOGE(TAG,"ConnAuthenticationFailed , login failed!\n");
			std::list<const LoginListener*>::iterator it = mLoginListenerList.begin();
			for(; it != mLoginListenerList.end(); it++)
			{
				if(NULL != (*it))
				{
					(*it)->onLogin(mUid,false);
				}
			}
		}else
		{
			skynet::MessageReceiverListener::ConnectionError error = skynet::MessageReceiverListener::ConnectionError::ConnOtherError;
			if(e == gloox::ConnectionError::ConnStreamClosed)
			{
				LOGE(TAG,"ConnStreamClosed , The stream has been closed (by the server)\n");
				//connection be closed
				error = skynet::MessageReceiverListener::ConnectionError::ConnStreamClosed;

			}else if(e == gloox::ConnectionError::ConnIoError)
			{
				// Send Message failed will call onDisconnect, ConnectionError == ConnIoError
				mIsSend = false;
				LOGE(TAG,"ConnIoError , An I/O error occured.\n");
				// io error
				error = skynet::MessageReceiverListener::ConnectionError::ConnIoError;
			}else if(e == gloox::ConnectionError::ConnUserDisconnected)
			{
				LOGD(TAG,"ConnUserDisconnected , The user (or higher-level protocol) requested a disconnect\n");
				// The user (or higher-level protocol) requested a disconnect.
				error = skynet::MessageReceiverListener::ConnectionError::ConnUserDisconnected;
			}else
			{
				// other error case
				LOGE(TAG,"other error case.\n");
			}
			std::list<const MessageReceiverListener*>::iterator it = mMessageReceiverListenerList.begin();
			for(;it != mMessageReceiverListenerList.end(); it++)
			{
				if(NULL != (*it))
				{
					(*it)->onDisConnect(error);
				}
			}
		}
	}

	bool SkynetImpl::onTLSConnect( const gloox::CertInfo& info )
	{
		LOGD(TAG,"status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n",
			info.status, info.issuer.c_str(), info.server.c_str(),
			info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
			info.compression.c_str());
		return true;
	}

	void SkynetImpl::handleMUCParticipantPresence( gloox::MUCRoom* room, const gloox::MUCRoomParticipant participant,
		const gloox::Presence& presence )
	{
		LOGD(TAG,"handleMUCParticipantPresence begin\n");
		if( presence.presence() == gloox::Presence::Available )
		{
			//local user
			std::string localUserName = room->nick();
			std::string joinUserName;
			// participant
			if(participant.nick != NULL)
			{
				joinUserName = participant.nick->resource();
			}else
			{
				if(participant.jid != NULL)
				{
					joinUserName = participant.jid->resource();
				}
			}
			
			LOGD(TAG,"handleMUCParticipantPresence Available ,localUserName: %s, joinUserName: %s\n", localUserName.c_str(),joinUserName.c_str());
			if(0 == strcmp(localUserName.c_str(),joinUserName.c_str()))
			{
				// room info
				room->getRoomItems();
			}
			LOGD(TAG,"!!!!!!!!!!!!!!!! %s is in the room, too\n", joinUserName.c_str());
		}

		else if( presence.presence() == gloox::Presence::Unavailable )
		{
			// other's leave the room message notification, self leave does not have this message notification
			//local user
			std::string localUserName = room->nick();
			std::string leaveUserName;
			// participant
			if(participant.nick != NULL)
			{
				leaveUserName = participant.nick->resource();
			}else
			{
				if(participant.jid != NULL)
				{
					leaveUserName = participant.jid->resource();
				}
			}
			
			LOGD(TAG,"handleMUCParticipantPresence Unavailable ,localUserName: %s, leaveUserName: %s\n", localUserName.c_str(),leaveUserName.c_str());
			LOGD(TAG,"!!!!!!!!!!!!!!!! %s left the room\n", leaveUserName.c_str());
			
		}

		else
		{
			LOGD(TAG,"Presence is %d of %s\n", presence.presence(), participant.jid->username().c_str() );
		}
		LOGD(TAG,"handleMUCParticipantPresence end\n");
	}

	void SkynetImpl::handleMUCMessage( gloox::MUCRoom* room, const gloox::Message& msg, bool priv )
	{
		LOGD(TAG,"handleMUCMessage begin\n");
		gloox::Message::MessageType msgType = msg.subtype();
		if(msgType == gloox::Message::MessageType::Groupchat || msgType == gloox::Message::MessageType::Normal)
		{
			gloox::JID from = msg.from();
			std::string fromUid = from.resource();
			// roomid
			std::string bare_from = from.bare();	
			std::string full_from = from.full();
			gloox::JID to = msg.to();
			std::string toUid = to.username();
			std::string bare_to = to.bare();
			std::string full_to = to.full();
			LOGD(TAG,"handleMUCMessage, MessageType : Groupchat, from: %s, to: %s\n",fromUid.c_str(), toUid.c_str());
			// 
			if(strcmp(fromUid.c_str(),toUid.c_str()) == 0)
			{
				// self send to self, do nothing
				LOGD(TAG,"handleMUCMessage, self send to self, just ignored!\n");
				return;
			}
			bool flag = false;
			std::map<std::string,gloox::MUCRoom*>::iterator iter = mJoinRoomMap.begin();
			std::string roomid;
			for(;iter != mJoinRoomMap.end(); iter++)
			{
				if((*iter).second== room)
				{
					flag = true;
					roomid = (*iter).first;
					break;
				}
			}
			if(flag)
			{

				std::list<const MessageReceiverListener*>::iterator it = mMessageReceiverListenerList.begin();
				for(;it != mMessageReceiverListenerList.end(); it++)
				{
					if(NULL != (*it))
					{
						std::string body = msg.body();
						LOGI(TAG,"handleMUCMessage, body: %s\n", body.c_str());
						skynet::MessageReceiverListener::MessageType type = skynet::MessageReceiverListener::MessageType::Groupchat;
						skynet::MessageReceiverListener::MucMessage msgTemp;
						msgTemp.from = fromUid;
						msgTemp.bare_from = bare_from;
						msgTemp.full_from = full_from;
						msgTemp.to = toUid;
						msgTemp.bare_to = bare_to;
						msgTemp.full_to = full_to;
						msgTemp.roomid = roomid;
						msgTemp.body = body;
						msgTemp.type = type;
						std::string id = msg.id();
						msgTemp.timestamp = MessageExt::getInstance()->timestamp(id);
						// just remove it
						MessageExt::getInstance()->remove(id);
						std::string msgId = mPacket.getPacketID();
						msgTemp.id = msgId;
						(*it)->onMUCReceive(msgTemp);
					}
				}
			}
		}else if(msgType == gloox::Message::MessageType::Error)
		{
			// 
			LOGD(TAG,"handleMUCMessage msgType: %d, Error message\n",msgType);
		}else if(msgType == gloox::Message::MessageType::Headline)
		{
			LOGD(TAG,"handleMUCMessage msgType: %d, Headline message\n",msgType);
		}else if(msgType == gloox::Message::MessageType::Groupchat)
		{
			LOGD(TAG,"handleMUCMessage msgType: %d, Groupchat message\n",msgType);
		}else if(msgType == gloox::Message::MessageType::Invalid)
		{
			LOGD(TAG,"handleMUCMessage msgType: %d, Invalid message\n",msgType);
		}
		else
		{
			// Never executed here
			LOGD(TAG,"handleMUCMessage Never executed here\n");
		}
		LOGD(TAG,"handleMUCMessage end\n");
	}
	bool SkynetImpl::handleMUCRoomCreation( gloox::MUCRoom* room )
	{
		LOGD(TAG,"handleMUCRoomCreation\n");
		return true;
	}
	void SkynetImpl::handleMUCSubject( gloox::MUCRoom* room, const std::string& nick,const std::string& subject )
	{
		LOGD(TAG,"handleMUCSubject\n");
	}
	void SkynetImpl::handleMUCInviteDecline( gloox::MUCRoom* room, const gloox::JID& invitee,const std::string& reason )
	{
		LOGD(TAG,"handleMUCInviteDecline\n");
	}
	void SkynetImpl::handleMUCError( gloox::MUCRoom* room, gloox::StanzaError error )
	{
		LOGE(TAG,"handleMUCError\n");
		std::string reason = "error";
		bool flag = false;
		std::map<std::string,gloox::MUCRoom*>::iterator iter = mJoinRoomMap.begin();
		std::string roomid;
		gloox::MUCRoom* roomTemp = NULL;
		for(;iter != mJoinRoomMap.end(); iter++)
		{
			if((*iter).second == room)
			{
				flag = true;
				roomid = (*iter).first;
				roomTemp = (*iter).second;
				break;
			}
		}
		if(error == gloox::StanzaError::StanzaErrorConflict)
		{
			reason = "an existing roomnick exists with the same name";
		}
		if(flag)
		{
			// should erase this listener,enable to join the room next time
			delete roomTemp;
			roomTemp = NULL;
			mJoinRoomMapMutex.lock();
			mJoinRoomMap.erase(iter);
			mJoinRoomMapMutex.unlock();
			std::list<const JoinRoomListener*>::iterator it = mJoinRoomListenerList.begin();
			for(; it != mJoinRoomListenerList.end(); it++)
			{
				if(NULL != (*it)) 
				{
					(*it)->onJoinFailed(roomid,reason);
				}
			}
		}
	}
	void SkynetImpl::handleMUCInfo( gloox::MUCRoom* room, int features, const std::string& name,const gloox::DataForm* infoForm )
	{
		LOGD(TAG,"handleMUCInfo\n");
	}
	void SkynetImpl::handleMUCItems( gloox::MUCRoom* room, const gloox::Disco::ItemList& items )
	{
		LOGD(TAG,"handleMUCItems\n");
		bool flag = false;
		std::map<std::string,gloox::MUCRoom*>::iterator iter = mJoinRoomMap.begin();
		std::string roomid;
		for(;iter != mJoinRoomMap.end(); iter++)
		{
			if((*iter).second == room)
			{
				flag = true;
				roomid = (*iter).first;
				break;
			}
		}
		if(flag)
		{
			std::list<const JoinRoomListener*>::iterator it = mJoinRoomListenerList.begin();
			for(; it != mJoinRoomListenerList.end(); it++)
			{
				if(NULL != (*it)) 
				{
					int count = items.size();
					(*it)->onJoinRoom(roomid,count);
				}
			}
		}	
	}

	void SkynetImpl::handlePresence( const gloox::Presence& presence )
	{
		LOGD(TAG,"handlePresence\n");
	}
}