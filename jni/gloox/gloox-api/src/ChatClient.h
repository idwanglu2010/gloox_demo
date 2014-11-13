#ifndef CHAT_CLIENT_H__
#define CHAT_CLIENT_H__
#include "../../gloox/src/gloox.h"
#include "../../gloox/src/client.h"
#include "LogConfig.h"
#include "MessageExt.h"

namespace skynet
{
	class ChatClient:public gloox::Client
	{
	public:
		/**
		* Constructs a new Client which can be used for account registration only.
		* SASL and TLS are on by default. The port will be determined by looking up SRV records.
		* Alternatively, you can set the port explicitly by calling @ref setPort().
		* @param server The server to connect to.
		*/
		ChatClient(const std::string& server);
		ChatClient( const gloox::JID& jid, const std::string& password);
		virtual void handleDisconnect( const gloox::ConnectionBase* connection, gloox::ConnectionError reason );
		virtual void handleTag( gloox::Tag* tag );
	private:
	};
}
#endif //CHAT_CLIENT_H__