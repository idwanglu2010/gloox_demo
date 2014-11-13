package com.idreamsky.gloox.interfaces;

public interface JoinRoomListener {
	public void onJoinRoom(String roomid, int count);
	public void onJoinFailed(String roomid, String reason);
}
