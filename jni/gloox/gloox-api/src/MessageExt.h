#ifndef MESSAGEEXT_H__
#define MESSAGEEXT_H__
#include <iostream>
#include "../../gloox/src/gloox.h"
#include "../../gloox/src/client.h"
#include <time.h>
namespace skynet
{
	class MessageExt
	{
	private:
		struct Message;
	public:
		static MessageExt* getInstance();
	public:
		std::string timestamp(const std::string & id) const;
		std::string body(const std::string & id) const;
		void handleTag( gloox::Tag* tag );
		void put(const struct Message& msg );
		void remove(const struct Message& msg);
		void remove(const std::string & id);
	private:
		MessageExt();
		~MessageExt();
	private:
		struct Message
		{
			std::string id;
			std::string body;
			std::string timestamp;

			bool operator==( const Message &right ) const
			{
				bool flag = false;
				if(this->id == right.id && this->body == right.body && this->timestamp == right.timestamp)
				{
					flag = true;
				}
				return flag;
			}
		};
	private:
		std::list<struct Message> mMessageList;
		gloox::util::Mutex mMessageListMutex;
		static gloox::util::Mutex sInscanceMutex;
		static MessageExt* sInscance;
	};
}


#endif // MESSAGEEXT_H__