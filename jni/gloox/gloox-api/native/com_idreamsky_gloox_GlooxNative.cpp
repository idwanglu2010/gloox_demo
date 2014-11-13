#include "com_idreamsky_gloox_GlooxNative.h"
#include "../../gloox-api/inc/Skynet.h"
#include "../../gloox-api/inc/LoginListener.h"
#include "../../gloox-api/inc/SendMessageListener.h"
#include "../../gloox-api/inc/MessageReceiverListener.h"
#include "../../gloox-api/inc/JoinRoomListener.h"
#include "../../gloox-api/inc/LeaveRoomListener.h"
#include "../../gloox-api/src/LogConfig.h"
#include <iostream>
using namespace skynet;
#define TAG "Chat"

static JavaVM * gJavaVM = NULL;
static jobject gNativeJavaObject = NULL;

static int getEnv(JNIEnv **env, bool* pIsAttached)
{
	*pIsAttached = false;
	int ret = -1;
	JNIEnv *jniEnv = NULL;
	ret = gJavaVM->GetEnv((void**)&jniEnv,JNI_VERSION_1_4);
	if(ret != JNI_OK)
	{
		LOGD(TAG,"getEnv GetEnv error ret: %d", ret);
		ret = gJavaVM->AttachCurrentThread(&jniEnv,NULL);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"getEnv AttachCurrentThread error ret: %d", ret);
		}else
		{
			*pIsAttached = true;
			LOGD(TAG,"getEnv GetEnv success AttachCurrentThread success");
		}
	}else
	{
		LOGD(TAG,"getEnv GetEnv success");
	}
	*env = jniEnv;
	return ret;
}

static int detachCurrentThread(bool isAttached)
{
	LOGD(TAG,"detachCurrentThread begin isAttached: %d",isAttached);
	int ret = -1;
    if(isAttached)
    {
    	ret = gJavaVM->DetachCurrentThread();
    	if(ret != JNI_OK)
    	{
    		LOGD(TAG,"DetachCurrentThread failed, ret: %d", ret);
    	}else
    	{
    		LOGD(TAG,"DetachCurrentThread success, ret: %d", ret);
    	}

    }else
    {
    	ret = JNI_OK;
    }
    LOGD(TAG,"detachCurrentThread end ret: %d",ret);
    return ret;
}

class JNINativeCallbackMethod
{
public:
	JNINativeCallbackMethod(const std::string& funName,const std::string& signature)
	{
		mFunName = funName;
		mSignature = signature;
	}
	~JNINativeCallbackMethod()
	{

	}

	jmethodID getMethodId() const
	{
		return mMethodId;
	}
	std::string getFunName() const
	{
		return mFunName;
	}
	std::string getSignature() const
	{
		return mSignature;
	}
	void setMethodId(const jmethodID& methodId)
	{
		mMethodId = methodId;
	}
	void setFunName(const std::string& funName)
	{
		mFunName = funName;
	}
	void setSignature(const std::string& signature)
	{
		mSignature = signature;
	}
private:
	std::string mFunName;
	std::string mSignature;
	jmethodID mMethodId;
};

class SkynetWrapper: public LoginListener,
		public SendMessageListener,
		public MessageReceiverListener,
		public LeaveRoomListener,
		public JoinRoomListener {
public:
	SkynetWrapper() {

	}
	virtual ~SkynetWrapper() {

	}

	virtual void onLogin(std::string& uid,bool isLogin) const {
		//printf("%d",isLogin);
		LOGD(TAG,"onLogin uid: %s, isLogin: %d", uid.c_str(), isLogin);
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onLogin getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onLogin env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onLogin");
		jmethodID methodId = method->getMethodId();
		jstring str = charStringtoJstring(env,uid.c_str());
		env->CallVoidMethod(gNativeJavaObject,methodId,str,isLogin);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onLogin end");
	}

	virtual void onSend(bool isSend, const std::string& toUid,
			const std::string& msgId) const {
		//printf("onSend %d,toUid:%s msgId:%s",isSend,toUid,msgId);
		LOGD(TAG,"onSend %d,toUid:%s msgId:%s", isSend, toUid.c_str(),msgId.c_str());
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onSend getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onSend env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onSend");
		jmethodID methodId = method->getMethodId();
		jstring str = charStringtoJstring(env,toUid.c_str());
		jstring str1 = charStringtoJstring(env,msgId.c_str());
		env->CallVoidMethod(gNativeJavaObject,methodId,isSend,str,str1);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onSend end");
	}

	virtual void onReceive(const MessageReceiverListener::Message& message) const {
		//printf("onReceive message:%s ",message);
		std::string body = message.body;
		LOGD(TAG,"onReceive message:%s ", body.c_str());
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onReceive getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onReceive env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onReceive");
		jmethodID methodId = method->getMethodId();
		jstring str = charStringtoJstring(env,body.c_str());
		env->CallVoidMethod(gNativeJavaObject,methodId,str);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onReceive end");
	}

	virtual void onMUCReceive(const MessageReceiverListener::MucMessage& message) const
	{
		std::string roomid = message.roomid;
		std::string body = message.body;
		LOGD(TAG,"onMUCReceive roomid: %s, message:%s ",roomid.c_str(),body.c_str());
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onMUCReceive getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onMUCReceive env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onMUCReceive");
		jmethodID methodId = method->getMethodId();
		jstring roomidstr = charStringtoJstring(env,roomid.c_str());
		jstring str = charStringtoJstring(env,body.c_str());
		env->CallVoidMethod(gNativeJavaObject,methodId,roomidstr,str);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onMUCReceive end");
	}

	virtual void onDisConnect(MessageReceiverListener::ConnectionError error) const {
		//printf("onDisConnect");
		LOGD(TAG,"onDisConnect");
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onDisConnect getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onDisConnect env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onDisConnect");
		jmethodID methodId = method->getMethodId();
		env->CallVoidMethod(gNativeJavaObject,methodId, error);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onDisConnect end");
	}

	virtual void onJoinRoom(const std::string& roomId, int count) const {
		//printf("onJoinRoom roomId:%s,count: %d",roomId,count);
		LOGD(TAG,"onJoinRoom roomId:%s,count: %d", roomId.c_str(), count);
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onJoinRoom getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onJoinRoom env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onJoinRoom");
		jmethodID methodId = method->getMethodId();
		jstring str = charStringtoJstring(env,roomId.c_str());
		env->CallVoidMethod(gNativeJavaObject,methodId,str,count);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onJoinRoom end");
	}

	virtual void onJoinFailed(const std::string& roomId,const std::string reason) const
	{
		//printf("onJoinFailed roomId:%s,reason: %s",roomId,reason);
		LOGD(TAG,"onJoinFailed roomId:%s,reason: %s", roomId.c_str(), reason.c_str());
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onJoinFailed getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onJoinFailed env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onJoinFailed");
		jmethodID methodId = method->getMethodId();
		jstring str = charStringtoJstring(env,roomId.c_str());
		jstring reasonstr = charStringtoJstring(env,reason.c_str());
		env->CallVoidMethod(gNativeJavaObject,methodId,str,reasonstr);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onJoinFailed end");
	}

	virtual void onLeaveRoom(const std::string& roomId) const
	{
		//printf("onLeaveRoom roomId:%s",roomId);
		LOGD(TAG,"onLeaveRoom roomId:%s", roomId.c_str());
		bool isAttached = false;
		JNIEnv *env = NULL;
		int ret = getEnv(&env,&isAttached);
		if(ret != JNI_OK)
		{
			LOGD(TAG,"onLeaveRoom getEnv failed");
			return;
		}
		if(NULL == env)
		{
			LOGD(TAG,"onLeaveRoom env is null");
			return;
		}
		const JNINativeCallbackMethod *method = getJNINativeCallbackMethod("onLeaveRoom");
		jmethodID methodId = method->getMethodId();
		jstring str = charStringtoJstring(env,roomId.c_str());
		env->CallVoidMethod(gNativeJavaObject,methodId,str);
		detachCurrentThread(isAttached);
		LOGD(TAG,"onLeaveRoom end");
	}
};

static SkynetWrapper *sListener = new SkynetWrapper();

/*
 * Class:     com_idreamsky_gloox_GlooxNative
 * Method:    initialize
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_idreamsky_gloox_GlooxNative_initialize
  (JNIEnv *env, jobject object, jint env_mode, jstring appid)
{
	gNativeJavaObject = env->NewGlobalRef(object);
	Skynet::EnvironmentType type = Skynet::EnvironmentType::Official;
	if(env_mode == 1)
	{
		type = Skynet::EnvironmentType::Dev;
	}else if(env_mode == 2)
	{
		type = Skynet::EnvironmentType::Test;
	}else if(env_mode == 3)
	{
		type = Skynet::EnvironmentType::Sandbox;
	}else if(env_mode == 5)
	{
		type = Skynet::EnvironmentType::Self;
	}
	char*appidCstr = jstringToCharstring(env,appid);
	LOGD(TAG,"initialize , env_mode: %d, appid: %s", env_mode, appidCstr);
	std::string appidStr;
	if(NULL != appidCstr)
	{
		appidStr = std::string(appidCstr);
		free(appidCstr);
	}
	Skynet::InitializeInfo info;
	info.type = type;
	info.appid = appidStr;
	Skynet::initialize(info);
	Skynet::registerMessageReceiverListener(sListener);
}

/*
 * Class:     com_idreamsky_gloox_GlooxNative
 * Method:    login
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_idreamsky_gloox_GlooxNative_login
  (JNIEnv *env, jobject object, jstring uid, jstring sessionid)
{
	char*uidCstr = jstringToCharstring(env,uid);
	char*sessionidCstr = jstringToCharstring(env,sessionid);
	LOGD(TAG,"login uid: %s,sessionid: %s",uidCstr,sessionidCstr);
	std::string uidStr;
	if(NULL != uidCstr)
	{
		uidStr = std::string(uidCstr);
	}
	std::string sessionidStr;
	if(NULL != sessionidCstr)
	{
		sessionidStr = std::string(sessionidCstr);
	}
	free(uidCstr);
	free(sessionidCstr);
	Skynet::LoginInfo loginInfo;
	loginInfo.uid = uidStr;
	loginInfo.sessionid = sessionidStr;
	Skynet::login(loginInfo,sListener);
}

/*
 * Class:     com_idreamsky_gloox_GlooxNative
 * Method:    sendMessage
 * Signature: (Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_idreamsky_gloox_GlooxNative_sendMessage
  (JNIEnv *env, jobject object, jstring to_uid, jstring text, jint msg_type)
{
	char*toUidCstr = jstringToCharstring(env,to_uid);
	char*textCstr = jstringToCharstring(env,text);
	Skynet::MessageType type = Skynet::MessageType::Chat;
	if(msg_type == 1)
	{
		type = Skynet::MessageType::Chat;
		LOGD(TAG,"sendMessage to_uid: %s,text: %s,msg_type: %d",toUidCstr,textCstr,msg_type);
	}else if(msg_type == 4)
	{
		type = Skynet::MessageType::Groupchat;
		LOGD(TAG,"sendMessage to_roomid: %s,text: %s,msg_type: %d",toUidCstr,textCstr,msg_type);
	}else
	{
		LOGE(TAG,"msg_type not support");
		return;
	}
	std::string toUidStr;
	if(NULL != toUidCstr)
	{
		toUidStr = std::string(toUidCstr);
	}
	std::string textStr;
	if(NULL != textCstr)
	{
		textStr = std::string(textCstr);
	}
	free(toUidCstr);
	free(textCstr);
	Skynet::SendMessageInfo info;
	info.toUid = toUidStr;
	info.body = textStr;
	info.type = type;
	Skynet::sendMessage(info,sListener);
}

/*
 * Class:     com_idreamsky_gloox_GlooxNative
 * Method:    joinChatRoom
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_idreamsky_gloox_GlooxNative_joinChatRoom
  (JNIEnv *env, jobject object, jstring roomid, jstring roomnick)
{
	char*roomidCstr = jstringToCharstring(env,roomid);
	char*roomnickCstr = jstringToCharstring(env,roomnick);
	std::string roomidStr;
	if(NULL != roomidCstr)
	{
		roomidStr = std::string(roomidCstr);
	}
	std::string roomnickStr;
	if(NULL != roomnickCstr)
	{
		roomnickStr = std::string(roomnickCstr);
	}
	LOGD(TAG,"joinChatRoom roomid: %s, roomnick: %s",roomidCstr, roomnickCstr);
	free(roomidCstr);
	free(roomnickCstr);
	Skynet::JoinChatRoomInfo info;
	info.roomid = roomidStr;
	info.roomnick = roomnickStr;
	Skynet::joinChatRoom(info,sListener);
}

/*
 * Class:     com_idreamsky_gloox_GlooxNative
 * Method:    leaveChatRoom
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_idreamsky_gloox_GlooxNative_leaveChatRoom
  (JNIEnv *env, jobject object, jstring roomid, jstring roomnick)
{
	LOGD(TAG,"leaveChatRoom");
	char*roomidCstr = jstringToCharstring(env,roomid);
	char*roomnickCstr = jstringToCharstring(env,roomnick);
	std::string roomidStr;
	if(NULL != roomidCstr)
	{
		roomidStr = std::string(roomidCstr);
	}
	std::string roomnickStr;
	if(NULL != roomnickCstr)
	{
		roomnickStr = std::string(roomnickCstr);
	}
	LOGD(TAG,"leaveChatRoom roomid: %s, roomnick: %s",roomidCstr,roomnickCstr);
	free(roomidCstr);
	free(roomnickCstr);
	Skynet::LeaveChatRoomInfo info;
	info.roomid = roomidStr;
	info.roomnick = roomnickStr;
	Skynet::leaveChatRoom(info,sListener);
}

/*
 * Class:     com_idreamsky_gloox_GlooxNative
 * Method:    logout
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_idreamsky_gloox_GlooxNative_logout
  (JNIEnv *env, jobject object)
{
	LOGD(TAG,"logout");
	Skynet::logout();
}

static JNINativeMethod gMethods[] = {
    {  "initialize", "(ILjava/lang/String;)V",
            (void *) Java_com_idreamsky_gloox_GlooxNative_initialize},
    {  "login", "(Ljava/lang/String;Ljava/lang/String;)V",
            (void *) Java_com_idreamsky_gloox_GlooxNative_login},
    {  "sendMessage", "(Ljava/lang/String;Ljava/lang/String;I)V",
            (void *) Java_com_idreamsky_gloox_GlooxNative_sendMessage},
    {  "joinChatRoom", "(Ljava/lang/String;Ljava/lang/String;)V",
            (void *) Java_com_idreamsky_gloox_GlooxNative_joinChatRoom},
	{  "leaveChatRoom", "(Ljava/lang/String;Ljava/lang/String;)V",
			(void *) Java_com_idreamsky_gloox_GlooxNative_leaveChatRoom},
	{  "logout", "()V",
			(void *) Java_com_idreamsky_gloox_GlooxNative_logout},
};

static JNINativeCallbackMethod gCallbackMethods[] = {
    {  "onLogin", "(Ljava/lang/String;Z)V"},
    {  "onSend", "(ZLjava/lang/String;Ljava/lang/String;)V"},
    {  "onJoinRoom", "(Ljava/lang/String;I)V"},
    {  "onJoinFailed", "(Ljava/lang/String;Ljava/lang/String;)V"},
    {  "onLeaveRoom", "(Ljava/lang/String;)V"},
	{  "onReceive", "(Ljava/lang/String;)V"},
	{  "onMUCReceive", "(Ljava/lang/String;Ljava/lang/String;)V"},
	{  "onDisConnect", "(I)V"},
};

const JNINativeCallbackMethod* getJNINativeCallbackMethod(const char* funcName)
{
	LOGD(TAG,"getJNINativeCallbackMethod begin,funcName: %s",funcName);
	JNINativeCallbackMethod *method = NULL;
	int size = sizeof(gCallbackMethods) / sizeof(JNINativeCallbackMethod);
	for(int i = 0; i < size; ++i)
	{
		const char* str = gCallbackMethods[i].getFunName().c_str();
		if(strcmp(funcName,str) == 0)
		{
			method =  &gCallbackMethods[i];
			break;
		}
	}
	LOGD(TAG,"getJNINativeCallbackMethod end,funcName: %s,method: %p",funcName,method);
	return method;
}

//jstring to char*
char* jstringToCharstring(JNIEnv* env, jstring jstr)
{
       char* rtn = NULL;
       jclass clsstring = env->FindClass("java/lang/String");
       jstring strencode = env->NewStringUTF("utf-8");
       jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
       jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
       jsize alen = env->GetArrayLength(barr);
       jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
       if (alen > 0)
       {
                 rtn = (char*)malloc(alen + 1);
                 memcpy(rtn, ba, alen);
                 rtn[alen] = 0;
       }
       env->ReleaseByteArrayElements(barr, ba, 0);
       return rtn;
}

//char* to jstring
jstring charStringtoJstring(JNIEnv* env, const char* pat)
{
       jclass strClass = env->FindClass("java/lang/String");
       jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
       jbyteArray bytes = env->NewByteArray(strlen(pat));
       env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
       jstring encoding = env->NewStringUTF("utf-8");
       return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

static int register_com_idreamsky_gloox_GlooxNative(JNIEnv* env)
{
	LOGD(TAG,"register_com_idreamsky_gloox_GlooxNative");
	jclass clazz = env->FindClass("com/idreamsky/gloox/GlooxNative");
	register_com_idreamsky_gloox_GlooxNativeCallbacks(env);
	return env->RegisterNatives(clazz, gMethods,sizeof(gMethods) / sizeof(JNINativeMethod));
}

static int register_com_idreamsky_gloox_GlooxNativeCallbacks(JNIEnv* env)
{
	int ret = -1;
	LOGD(TAG,"register_com_idreamsky_gloox_GlooxNativeCallbacks");
	jclass clazz = env->FindClass("com/idreamsky/gloox/GlooxNative");
	int size = sizeof(gCallbackMethods) / sizeof(JNINativeCallbackMethod);
	for(int i = 0; i< size; ++i)
	{
		const char* funName = gCallbackMethods[i].getFunName().c_str();
		const char* signature = gCallbackMethods[i].getSignature().c_str();
		LOGD(TAG,"funName %s",funName);
		LOGD(TAG,"signature %s",signature);
		jmethodID methodId = env->GetMethodID(clazz, funName, signature);
		LOGD(TAG,"methodId %d",methodId);
		gCallbackMethods[i].setMethodId(methodId);
	}
	LOGD(TAG,"register_com_idreamsky_gloox_GlooxNativeCallbacks end");
	return ret;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv *env = NULL;
	gJavaVM = vm;
	LOGD(TAG,"JNI_OnLoad begin");
	if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
		return JNI_ERR;
	}
	if (register_com_idreamsky_gloox_GlooxNative(env)) {
		return JNI_ERR;
	}
	LOGD(TAG,"JNI_OnLoad success end");
	return JNI_VERSION_1_4;
}

