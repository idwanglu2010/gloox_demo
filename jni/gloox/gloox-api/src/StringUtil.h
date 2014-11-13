#ifndef STRINGUTIL_H__
#define STRINGUTIL_H__
#include <iostream>
namespace skynet
{
	class StringUtil
	{
	public:
		StringUtil();
		~StringUtil();
		static std::string randomString(int len);
		static int randNum(int max, int min);
	private:
		static const char* mNumbersAndLetters;
	};
}


#endif // STRINGUTIL_H__