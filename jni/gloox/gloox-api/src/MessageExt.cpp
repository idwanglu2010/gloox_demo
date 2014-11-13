#include "MessageExt.h"
#include "LogConfig.h"
#define TAG "MessageExt"
namespace skynet
{
	MessageExt* MessageExt::sInscance = NULL;
	gloox::util::Mutex MessageExt::sInscanceMutex;

	MessageExt::MessageExt()
	{
	}

	MessageExt::~MessageExt()
	{
	}

	MessageExt* MessageExt::getInstance()
	{
		if(NULL == sInscance)
		{
			sInscanceMutex.lock();
			if(NULL == sInscance)
			{
				sInscance = new MessageExt();
			}
			sInscanceMutex.unlock();
		}
		return sInscance;
	}

	std::string MessageExt::timestamp(const std::string & id) const
	{
		struct Message msg;
		std::list<struct Message>::const_iterator iter = mMessageList.begin();
		for(; iter != mMessageList.end(); ++iter)
		{
			if((*iter).id == id)
			{
				msg = *iter;
				break;
			}
		}
		return msg.timestamp;
	}
	std::string MessageExt::body(const std::string & id) const
	{
		struct Message msg;
		std::list<struct Message>::const_iterator iter = mMessageList.begin();
		for(; iter != mMessageList.end(); ++iter)
		{
			if((*iter).id == id)
			{
				msg = *iter;
				break;
			}
		}
		return msg.body;
	}
	void MessageExt::handleTag( gloox::Tag* tag )
	{
		if(0 == tag)
		{
			return;
		}
		struct Message msg;
		msg.id = tag->findAttribute("id");
		gloox::Tag* temp = tag->findChild("body");
		if(0 != temp)
		{
			msg.body = temp->cdata();
		}
		temp = tag->findChild("ext");
		if(0 != temp)
		{
			temp = temp->findChild("time");
			if(0 != temp)
			{
				msg.timestamp = temp->cdata();	
			}
		}else
		{
			// server does not provide timestamp, so generate loacal machine timestamp instead
			time_t timep;
			time(&timep);
			char buffer[256];
			memset(buffer, 0, 256);
			sprintf( buffer , "%lld", timep);
			std::string timestamp(buffer);
			msg.timestamp = timestamp;
		}
		put(msg);
	}
	void MessageExt::put(const struct Message& msg )
	{
		mMessageListMutex.lock();
		mMessageList.push_back(msg);
		mMessageListMutex.unlock();
	}
	void MessageExt::remove(const struct Message& msg)
	{
		mMessageListMutex.lock();
		mMessageList.remove(msg);
		mMessageListMutex.unlock();
	}
	void MessageExt::remove(const std::string & id)
	{
		
		bool flag = false;
		struct Message msg;
		std::list<struct Message>::const_iterator iter = mMessageList.begin();
		for(; iter != mMessageList.end(); ++iter)
		{
			if((*iter).id == id)
			{
				msg = *iter;
				flag = true;
				break;
			}
		}
		if(flag)
		{
			remove(msg);
		}
	}
}
