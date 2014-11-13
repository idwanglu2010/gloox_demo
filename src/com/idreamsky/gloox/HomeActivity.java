package com.idreamsky.gloox;

import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;

import com.idreamsky.gloox.interfaces.JoinRoomListener;
import com.idreamsky.gloox.interfaces.LeaveRoomListener;
import com.idreamsky.gloox.interfaces.MessageReceiverListener;
import com.idreamsky.gloox.interfaces.SendMessageListener;

public class HomeActivity extends BaseActivity implements OnClickListener {

	private static String defaultChatUid = "123456789";
	private static String defaultChatMessage = "hello jony";
	private static String defaultGroupChatId = "1@cofhistory.johnny-pc";
	private static String defaultGroupChatMessage = "hello everyone";
	private static String defaultJoinRoomId = "1@cofhistory.johnny-pc";
	private static String defaultJoinRoomNick = "nick.wang";
	private static String defaultLeaveRoomId = "1@cofhistory.johnny-pc";
	private static String defaultLeaveRoomNick = "nick.wang";
	private EditText mChatEdit;
	private EditText mChatMessageEdit;
	private EditText mGroupChatEdit;
	private EditText mGroupChatMessageEdit;
	private EditText mJoinRoomEdit;
	private EditText mJoinRoomNickEdit;
	private EditText mLeaveRoomEdit;
	private EditText mLeaveRoomNickEdit;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		findViewById(R.id.sendChat).setOnClickListener(this);
		findViewById(R.id.sendGroutChat).setOnClickListener(this);
		findViewById(R.id.joinChatRoom).setOnClickListener(this);
		findViewById(R.id.leaveChatRoom).setOnClickListener(this);
		findViewById(R.id.logout).setOnClickListener(this);
		mChatEdit = (EditText) findViewById(R.id.sendChatEdit);
		mChatEdit.setHint(defaultChatUid);
		mChatEdit.setText(defaultChatUid);
		mChatMessageEdit = (EditText) findViewById(R.id.sendChatMessageEdit);
		mChatMessageEdit.setHint(defaultChatMessage);
		mChatMessageEdit.setText(defaultChatMessage);
		mGroupChatEdit = (EditText) findViewById(R.id.sendGroupChatEdit);
		mGroupChatEdit.setHint(defaultGroupChatId);
		mGroupChatEdit.setText(defaultGroupChatId);
		mGroupChatMessageEdit = (EditText) findViewById(R.id.sendGroupChatMessageEdit);
		mGroupChatMessageEdit.setHint(defaultGroupChatMessage);
		mGroupChatMessageEdit.setText(defaultGroupChatMessage);
		mJoinRoomEdit = (EditText) findViewById(R.id.joinChatRoomEdit);
		mJoinRoomEdit.setHint(defaultJoinRoomId);
		mJoinRoomEdit.setText(defaultJoinRoomId);
		mJoinRoomNickEdit = (EditText)findViewById(R.id.joinChatRoomNickEdit);
		mJoinRoomNickEdit.setHint(defaultJoinRoomNick);
		mJoinRoomNickEdit.setText(defaultJoinRoomNick);	
		mLeaveRoomEdit = (EditText)findViewById(R.id.leaveChatRoomEdit);
		mLeaveRoomEdit.setHint(defaultLeaveRoomId);
		mLeaveRoomEdit.setText(defaultLeaveRoomId);
		mLeaveRoomNickEdit = (EditText)findViewById(R.id.leaveChatRoomNickEdit);
		mLeaveRoomNickEdit.setHint(defaultLeaveRoomNick);
		mLeaveRoomNickEdit.setText(defaultLeaveRoomNick);

		Gloox.getInstance().registerMessageReceiverListener(new MessageReceiverListener() {

			@Override
			public void onReceive(String message) {
				showToast("onReceive,message: " + message);
			}

			@Override
			public void onDisConnect(int errorCode) {
				String str;
				if(errorCode == Gloox.CONN_IO_ERROR)
				{
					str = "IO Error";
				}else if(errorCode == Gloox.CONN_STREAM_CLOSED)
				{
					str = "Connection is closed by server";
				}else if(errorCode == Gloox.CONN_USER_DISCONNECTED)
				{
					str = "Connection is closed by user";
				}else
				{
					str = "other error";
				}
				showToast("onDisconnect, reason: " + str);
				HomeActivity.this.finish();
			}

			@Override
			public void onMUCReceive(String roomid, String message) {
				showToast("onMUCReceive, roomid: " + roomid + ",message: " + message);
			}
		});
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.sendChat:
			if (!TextUtils.isEmpty(mChatEdit.getText().toString())) {
				defaultChatUid = mChatEdit.getText().toString();
			}
			if (!TextUtils.isEmpty(mChatMessageEdit.getText().toString())) {
				defaultChatMessage = mChatMessageEdit.getText().toString();
			}
			Gloox.getInstance().sendMessage(defaultChatUid, defaultChatMessage, Gloox.MSG_TYPE_CHAT, new SendMessageListener() {

				@Override
				public void onSend(boolean isSend, String uid, String msgId) {
					showToast("onSend isSend:" + isSend + ",uid:" + uid + ",msgId:" + msgId);
				}
			});
			break;
		case R.id.sendGroutChat:
			if (!TextUtils.isEmpty(mGroupChatEdit.getText().toString())) {
				defaultGroupChatId = mGroupChatEdit.getText().toString();
			}
			if (!TextUtils.isEmpty(mGroupChatMessageEdit.getText().toString())) {
				defaultGroupChatMessage = mGroupChatMessageEdit.getText().toString();
			}
			Gloox.getInstance().sendMessage(defaultGroupChatId, defaultGroupChatMessage, Gloox.MSG_TYPE_GROUPCHAT, new SendMessageListener() {

				@Override
				public void onSend(boolean isSend, String uid, String msgId) {
					showToast("onSend isSend:" + isSend + ",uid:" + uid + ",msgId:" + msgId);
				}
			});
			break;
		case R.id.joinChatRoom:
			if (!TextUtils.isEmpty(mJoinRoomEdit.getText().toString())) {
				defaultJoinRoomId = mJoinRoomEdit.getText().toString();
			}
			if (!TextUtils.isEmpty(mJoinRoomNickEdit.getText().toString())) {
				defaultJoinRoomNick = mJoinRoomNickEdit.getText().toString();
			}
			Gloox.getInstance().joinChatRoom(defaultJoinRoomId, defaultJoinRoomNick, new JoinRoomListener() {

				@Override
				public void onJoinRoom(String roomid, int count) {
					showToast("onJoinRoom roomid:" + roomid + ",count:" + count);
				}

				@Override
				public void onJoinFailed(String roomid, String reason) {
					showToast("onJoinFailed roomid:" + roomid + ",reason:" + reason);
				}
			});
			break;
		case R.id.leaveChatRoom:
			if (!TextUtils.isEmpty(mLeaveRoomEdit.getText().toString())) {
				defaultLeaveRoomId = mLeaveRoomEdit.getText().toString();
			}
			if (!TextUtils.isEmpty(mLeaveRoomNickEdit.getText().toString())) {
				defaultLeaveRoomNick = mLeaveRoomNickEdit.getText().toString();
			}
			Gloox.getInstance().leaveChatRoom(defaultLeaveRoomId, defaultLeaveRoomNick, new LeaveRoomListener() {
				
				@Override
				public void onLeaveRoom(String roomid) {
					showToast("onLeaveRoom roomid:" + roomid);
				}
			});
			break;
		case R.id.logout:
//			new Thread(new Runnable() {
//				
//				@Override
//				public void run() {
//					Gloox.getInstance().logout("123456", null);
//				}
//			}).start();
			Gloox.getInstance().logout(null);
			this.finish();
			break;
		default:
			break;
		}
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if(keyCode == KeyEvent.KEYCODE_BACK)
		{
			this.finish();
			return true;
			
		}else {
			return super.onKeyDown(keyCode, event);
		}	
	}
}
