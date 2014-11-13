#include "ChatClient.h"
#include <iostream>

#define TAG "ChatClient"

namespace skynet
{
	ChatClient::ChatClient(const std::string& server)
		:gloox::Client(server)
	{

	}

	ChatClient::ChatClient( const gloox::JID& jid, const std::string& password)
		:gloox::Client(jid,password)
	{

	}

	void ChatClient::handleDisconnect( const gloox::ConnectionBase* connection, gloox::ConnectionError reason )
	{
		gloox::Client::handleDisconnect( connection, reason);
	}

	void ChatClient::handleTag( gloox::Tag* tag )
	{
		LOGD(TAG,"handleTag, tag: %s\n",tag->xml().c_str());
		if( tag->name() == "message")
		{
			// just for get timestamp
			MessageExt::getInstance()->handleTag(tag);
		}
		gloox::Client::handleTag(tag);
	}
}