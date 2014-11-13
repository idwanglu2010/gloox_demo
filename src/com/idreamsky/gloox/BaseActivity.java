package com.idreamsky.gloox;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.Toast;

public class BaseActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if(keyCode == KeyEvent.KEYCODE_BACK)
		{
			android.os.Process.killProcess(android.os.Process.myPid());
			return true;
			
		}else {
			return super.onKeyDown(keyCode, event);
		}	
	}

	public void showToast(final String message) {
		this.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Toast.makeText(BaseActivity.this, message, Toast.LENGTH_LONG).show();

			}
		});
	}
}
