package com.idreamsky.gloox.interfaces;

public interface MessageReceiverListener {
	public void onReceive(String message);

	public void onMUCReceive(String roomid, String message);

	public void onDisConnect(int errorCode);
}
