package com.idreamsky.gloox;

public interface GlooxListener {
	public void onLogin(String uid, boolean isLogin);

	public void onJoinRoom(String roomid, int count);
	
	public void onJoinFailed(String roomid, String reason);

	public void onLeaveRoom(String roomid);

	public void onReceive(String message);

	public void onMUCReceive(String roomid, String message);

	public void onDisConnect(int errorCode);

	public void onSend(boolean isSend, String uid, String msgId);
}