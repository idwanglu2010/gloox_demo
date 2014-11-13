package com.idreamsky.gloox;

public class GlooxNative {

	static {
		// try {
		System.loadLibrary("gloox");
		// } catch (UnsatisfiedLinkError e) {
		// e.printStackTrace();
		// }
	}

	private static final Object synObject = new Object();
	private static GlooxNative sInstance = null;

	private GlooxListener mListener;

	private GlooxNative() {
	}

	public static GlooxNative getInstance() {
		if (null == sInstance) {
			synchronized (synObject) {
				if (null == sInstance) {
					sInstance = new GlooxNative();
				}
			}
		}
		return sInstance;
	}

	/**
	 * JNI begin
	 */
	public native void initialize(int environmentMode, String appid);

	public native void login(String uid, String sessionid);

	public native void sendMessage(String toUid, String text, int msgType);

	public native void joinChatRoom(String roomid, String roomnick);

	public native void leaveChatRoom(String roomid, String roomnick);

	public native void logout();

	// native call
	public void onLogin(String uid, boolean isLogin) {
		if (null != mListener) {
			mListener.onLogin(uid, isLogin);
		}
	}

	// native call
	public void onSend(boolean isSend, String uid, String msgId) {
		if (null != mListener) {
			mListener.onSend(isSend, uid, msgId);
		}
	}

	// native call
	public void onJoinRoom(String roomid, int count) {
		if (null != mListener) {
			mListener.onJoinRoom(roomid, count);
		}
	}

	// native call
	public void onJoinFailed(String roomid, String reason) {
		if (null != mListener) {
			mListener.onJoinFailed(roomid, reason);
		}
	}

	// native call
	public void onLeaveRoom(String roomid) {
		if (null != mListener) {
			mListener.onLeaveRoom(roomid);
		}
	}

	// native call
	public void onReceive(String message) {
		if (null != mListener) {
			mListener.onReceive(message);
		}
	}

	// native call
	public void onMUCReceive(String roomid, String message) {
		if (null != mListener) {
			mListener.onMUCReceive(roomid, message);
		}
	}

	// native call
	public void onDisConnect(int errorCode) {
		if (null != mListener) {
			mListener.onDisConnect(errorCode);
		}
	}

	/**
	 * JNI end
	 */

	public void setGlooxListener(GlooxListener mListener) {
		this.mListener = mListener;
	}
}
