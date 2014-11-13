package com.idreamsky.gloox;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;

import com.idreamsky.gloox.interfaces.LoginListener;

public class LoginActivity extends BaseActivity implements OnClickListener {

	private static String defaultLoginUid = "123456852";
	private EditText mLoginEdit;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login);
		findViewById(R.id.showLogin).setOnClickListener(this);
		mLoginEdit = (EditText) findViewById(R.id.showLoginEdit);
		mLoginEdit.setHint(defaultLoginUid);
		mLoginEdit.setText(defaultLoginUid);
		Gloox.getInstance().initialize(Gloox.ENV_SELF_MODE, "");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.showLogin:
			if (!TextUtils.isEmpty(mLoginEdit.getText().toString())) {
				defaultLoginUid = mLoginEdit.getText().toString();
			}
//			Thread thread = new Thread(new Runnable() {
//				
//				@Override
//				public void run() {
//					Gloox.getInstance().login(defaultLoginUid, "123456", "123456", new LoginListener() {
//
//						@Override
//						public void onLogin(String uid,boolean isLogin) {
//							showToast("onLogin uid: " + uid +",isLogin:" + isLogin);
//							Intent intent = new Intent();
//							intent.setClass(LoginActivity.this, HomeActivity.class);
//							startActivity(intent);
//							//LoginActivity.this.finish();
//						}
//					});
//				}
//			});
//			thread.start();
			Gloox.getInstance().login(defaultLoginUid, "", new LoginListener() {

				@Override
				public void onLogin(String uid,boolean isLogin) {
					showToast("onLogin uid: " + uid +",isLogin:" + isLogin);
					if(isLogin)
					{
						Intent intent = new Intent();
						intent.setClass(LoginActivity.this, HomeActivity.class);
						startActivity(intent);
						//LoginActivity.this.finish();
					}else {
						showToast("登录失败");
					}
				}
			});
			break;
		default:
			break;
		}
	}
}
