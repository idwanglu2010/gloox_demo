#ifndef MESSAGERECEIVERLISTENER_H__
#define MESSAGERECEIVERLISTENER_H__
#include <string>
namespace skynet 
{
	class MessageReceiverListener
	{
	public:
		enum MessageType
		{
			Chat           =  0,        /**< A chat message. */
			Groupchat      =  1,        /**< A groupchat message. */
		};
		struct Message
		{
			MessageType type;
			// from uid, 123456789
			std::string from;
			//from uid@host, 123456789@nick-pc
			std::string bare_from;
			//from uid@host/resource, 123456789@nick-pc/nw
			std::string full_from;
			// to uid, 123456852
			std::string to;
			//to uid@host, 123456852@nick-pc
			std::string bare_to;
			//to uid@host/resource, 123456852@nick-pc/nw
			std::string full_to;
			// message content
			std::string body;
			// message timestamp
			std::string timestamp;
			// msgid
			std::string id;
		};
		struct MucMessage
		{
			MessageType type;
			// from uid, 123456789
			std::string from;
			// equals roomid, 1@cofhistory.nick-pc
			std::string bare_from;
			// roomid/uid, 1@cofhistory.nick-pc/123456789
			std::string full_from;
			// to uid, 123456852
			std::string to;
			//to uid@host, 123456852@nick-pc
			std::string bare_to;
			//to uid@host/resource, 123456852@nick-pc/nw
			std::string full_to;
			// from roomid, equals bare_from, 1@cofhistory.nick-pc
			std::string roomid;
			// message content
			std::string body;
			// message timestamp
			std::string timestamp;
			// msgid
			std::string id;
		};
		enum ConnectionError
		{
			ConnStreamClosed            =  1,
			ConnIoError					=  2,
			ConnUserDisconnected		=  3,
			ConnOtherError				=  4,
		};
		virtual ~MessageReceiverListener() {}
		virtual void onReceive(const Message& message) const = 0;
		virtual void onMUCReceive(const MucMessage& message) const = 0;
		virtual void onDisConnect(ConnectionError error) const = 0;
	private:

	};
}

#endif //MESSAGERECEIVERLISTENER_H__