#ifndef LOGCONFIG_H__
#define LOGCONFIG_H__
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __ANDROID__
#include <android/log.h>
	#define LOGV(tag,...) __android_log_print(ANDROID_LOG_VERBOSE,tag ,__VA_ARGS__)
	#define LOGD(tag,...) __android_log_print(ANDROID_LOG_DEBUG,tag ,__VA_ARGS__)
	#define LOGI(tag,...) __android_log_print(ANDROID_LOG_INFO,tag ,__VA_ARGS__)
	#define LOGW(tag,...) __android_log_print(ANDROID_LOG_WARN,tag ,__VA_ARGS__)
	#define LOGE(tag,...) __android_log_print(ANDROID_LOG_ERROR,tag ,__VA_ARGS__)
	#define LOGF(tag,...) __android_log_print(ANDROID_LOG_FATAL,tag ,__VA_ARGS__)

	#define LOGVV(tag,format,...) __android_log_print(ANDROID_LOG_VERBOSE,tag ,"%s(%d)-<%s>:"#format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
	#define LOGDV(tag,format,...) __android_log_print(ANDROID_LOG_DEBUG,tag ,"%s(%d)-<%s>:"#format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
	#define LOGIV(tag,format,...) __android_log_print(ANDROID_LOG_INFO,tag ,"%s(%d)-<%s>:"#format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
	#define LOGWV(tag,format,...) __android_log_print(ANDROID_LOG_WARN,tag ,"%s(%d)-<%s>:"#format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
	#define LOGEV(tag,format,...) __android_log_print(ANDROID_LOG_ERROR,tag ,"%s(%d)-<%s>:"#format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
	#define LOGFV(tag,format,...) __android_log_print(ANDROID_LOG_FATAL,tag ,"%s(%d)-<%s>:"#format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)


#elif defined (_WIN32)
	#define LOGV(tag,...)	printf(__VA_ARGS__)
	#define LOGD(tag,...)	printf(__VA_ARGS__)
	#define LOGI(tag,...)	printf(__VA_ARGS__)
	#define LOGW(tag,...)	printf(__VA_ARGS__)
	#define LOGE(tag,...)	printf(__VA_ARGS__)
	#define LOGF(tag,...)	printf(__VA_ARGS__)

	#define LOGVV(...)
	#define LOGDV(...)
	#define LOGIV(...)
	#define LOGWV(...)
	#define LOGEV(...)
	#define LOGFV(...)
#else
	#define LOGV(...)
	#define LOGD(...)
	#define LOGI(...)
	#define LOGW(...)
	#define LOGE(...)
	#define LOGF(...)

	#define LOGVV(...)
	#define LOGDV(...)
	#define LOGIV(...)
	#define LOGWV(...)
	#define LOGEV(...)
	#define LOGFV(...)
#endif
#ifdef __cplusplus
}
#endif
#endif
