#ifndef SENDMESSAGELISTENER_H__
#define SENDMESSAGELISTENER_H__
#include <string>

namespace skynet 
{ 
	class SendMessageListener
	{
	public:
		virtual ~SendMessageListener() {}
		virtual void onSend(bool isSend, const std::string& toUid,const std::string& msgId) const = 0;
	private:

	};
}

#endif // SENDMESSAGELISTENER_H__