#ifndef LOGINLISTENER_H__
#define LOGINLISTENER_H__
#include <iostream>

namespace skynet {
	class LoginListener
	{
	public:
		virtual ~LoginListener() {}
		virtual void onLogin(std::string& uid,bool isLogin) const = 0;
	private:

	};
}


#endif // LOGINLISTENER_H__