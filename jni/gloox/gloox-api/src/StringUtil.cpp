#include "StringUtil.h"
#include "LogConfig.h"
#include <ctime>
#include <cstdlib>
namespace skynet
{
#define TAG "StringUtil"

	const char* StringUtil::mNumbersAndLetters = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	StringUtil::StringUtil()
	{
	}

	StringUtil::~StringUtil()
	{
	}


	std::string StringUtil::randomString(int len)
	{
		if(len < 1)
		{
			return NULL;
		}
#ifdef __ANDROID__
		srand48( time( 0 ) );
#else
		srand(time(0));
#endif
		char* arrayOfChar = new char[len + 1];
		int max = strlen(mNumbersAndLetters) - 1;
		for(int i = 0; i < len; ++i)
		{
			arrayOfChar[i] = mNumbersAndLetters[randNum(max,0)];
		}
		arrayOfChar[len] = '\0';
		std::string str(arrayOfChar);
		delete[] arrayOfChar;

		LOGI(TAG,"randomString, str: %s", str.c_str());
		return str;
	}

	// [min, max]
	int StringUtil::randNum(int max, int min)
	{
		int u = 0;
#ifdef __ANDROID__
		u = lrand48() % (max - min + 1) + min;
#else
		u = rand() % (max - min + 1) + min;
#endif

		LOGI(TAG,"randNum, u: %d", u);
		return u;
	}
}
