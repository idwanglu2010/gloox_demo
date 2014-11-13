package com.idreamsky.gloox;

import java.util.ArrayList;

import android.util.Log;

import com.idreamsky.gloox.interfaces.JoinRoomListener;
import com.idreamsky.gloox.interfaces.LeaveRoomListener;
import com.idreamsky.gloox.interfaces.LoginListener;
import com.idreamsky.gloox.interfaces.LogoutListener;
import com.idreamsky.gloox.interfaces.MessageReceiverListener;
import com.idreamsky.gloox.interfaces.SendMessageListener;

public class Gloox implements GlooxListener {

	private static final String TAG = Gloox.class.getSimpleName();
	public static final int MSG_TYPE_CHAT = 1;
	public static final int MSG_TYPE_GROUPCHAT = 4;
	
	public static final int ENV_DEV_MODE = 1;
	public static final int ENV_TEST_MODE = 2;
	public static final int ENV_SANDBOX_MODE = 3;
	public static final int ENV_OFFICIAL_MODE = 4;
	public static final int ENV_SELF_MODE = 5;
	
	public static final int CONN_STREAM_CLOSED = 1;
	public static final int CONN_IO_ERROR = 2;
	public static final int CONN_USER_DISCONNECTED = 3;
	public static final int CONN_OTHER_ERROR = 4;
	
	private static final Object synObject = new Object();
	private static final Object synObj = new Object();
	private static Gloox sInstance = null;

	private GlooxNative mGlooxNative;
	private ArrayList<MessageReceiverListener> mMessageReceiverListenerList = new ArrayList<MessageReceiverListener>();
	private LoginListener mLoginListener;
	private SendMessageListener mSendMessageListener;
	private JoinRoomListener mJoinRoomListener;
	private LeaveRoomListener mLeaveRoomListener;

	private Gloox() {
		mGlooxNative = GlooxNative.getInstance();
		mGlooxNative.setGlooxListener(this);
	}

	public static Gloox getInstance() {
		if (null == sInstance) {
			synchronized (synObject) {
				if (null == sInstance) {
					sInstance = new Gloox();
				}
			}
		}
		return sInstance;
	}

	/**
	 * public interface
	 */
	public void initialize(int environmentMode, String appid) {
		mGlooxNative.initialize(environmentMode, appid);
	}

	public void registerMessageReceiverListener(MessageReceiverListener listener) {
		synchronized (synObj) {
			if (!mMessageReceiverListenerList.contains(listener)) {
				mMessageReceiverListenerList.add(listener);
			}
		}
	}

	public void unregisterMessageReceiverListener(MessageReceiverListener listener) {
		synchronized (synObj) {
			mMessageReceiverListenerList.remove(listener);
		}
	}

	public void login(String uid, String sessionid,LoginListener listener) {
		mLoginListener = listener;
		mGlooxNative.login(uid, sessionid);
	}

	public void sendMessage(String toUid, String text, int msgType, SendMessageListener listener) {
		mSendMessageListener = listener;
		mGlooxNative.sendMessage(toUid, text, msgType);

	}

	public void joinChatRoom(String roomid, String roomnick, JoinRoomListener listener) {
		mJoinRoomListener = listener;
		mGlooxNative.joinChatRoom(roomid, roomnick);
	}

	public void leaveChatRoom(String roomid, String roomnick, LeaveRoomListener listener) {
		mLeaveRoomListener = listener;
		mGlooxNative.leaveChatRoom(roomid, roomnick);
	}

	public void logout(LogoutListener listener) {
		mGlooxNative.logout();
	}

	@Override
	public void onLogin(String uid,boolean isLogin) {
		if (null != mLoginListener) {
			mLoginListener.onLogin(uid,isLogin);
		}
	}

	@Override
	public void onJoinRoom(String roomid, int count) {
		if (null != mJoinRoomListener) {
			mJoinRoomListener.onJoinRoom(roomid, count);
		}
	}

	@Override
	public void onJoinFailed(String roomid, String reason) {
		if (null != mJoinRoomListener) {
			mJoinRoomListener.onJoinFailed(roomid, reason);
		}
	}
	
	@Override
	public void onLeaveRoom(String roomid) {
		if (null != mLeaveRoomListener) {
			mLeaveRoomListener.onLeaveRoom(roomid);
		}

	}

	@Override
	public void onReceive(String message) {
		for (int i = 0; i < mMessageReceiverListenerList.size(); i++) {
			mMessageReceiverListenerList.get(i).onReceive(message);
		}
	}

	@Override
	public void onDisConnect(int errorCode) {
		for (int i = 0; i < mMessageReceiverListenerList.size(); i++) {
			mMessageReceiverListenerList.get(i).onDisConnect(errorCode);
		}
	}

	@Override
	public void onSend(boolean isSend, String uid, String msgId) {
		if (null != mSendMessageListener) {
			mSendMessageListener.onSend(isSend, uid, msgId);
		}

	}

	@Override
	public void onMUCReceive(String roomid, String message) {
		Log.i(TAG, "onMUCReceive, message: "+ message);
		for (int i = 0; i < mMessageReceiverListenerList.size(); i++) {
			mMessageReceiverListenerList.get(i).onMUCReceive(roomid, message);
		}
	}

}
