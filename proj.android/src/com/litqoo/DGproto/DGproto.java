/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.litqoo.DGproto;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxRenderer;

import com.litqoo.lib.KSActivityBase;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManager;

public class DGproto extends KSActivityBase{//Cocos2dxActivity{
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		if(com.litqoo.lib.hspConnector.setup(10226, "SKDRAWGIRLS", "1.0.0.KG")){
			Log.i("com.litqoo.dgproto", "hspcore create ok");
		}else{
			Log.i("com.litqoo.dgproto","hspcore create fail");
		}
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//		
//		setContentView(R.layout.game_demo);
//		mGLView = (Cocos2dxGLSurfaceView) findViewById(R.id.game_gl_surfaceview);
//		mGLView.setEGLContextClientVersion(2);
//		mGLView.setCocos2dxRenderer(new Cocos2dxRenderer());
//		mGLView.setCocos2dxEditText((Cocos2dxEditText)findViewById(R.id.textField));
	}
	
//	public Cocos2dxGLSurfaceView onCreateGLSurfaceView() {
//		return new LuaGLSurfaceView(this);
//    }

    static { 
        System.loadLibrary("cocos2dlua");
    }
    
    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// hspConnector should create stencil buffer
    	mGLView = glSurfaceView;
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	com.litqoo.lib.hspConnector.kInit(this,glSurfaceView,getApplicationContext());
    	
    	return glSurfaceView;
    }
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
    	com.litqoo.lib.hspConnector.onActivityResult(requestCode, resultCode, data, this);
    }
}

class LuaGLSurfaceView extends Cocos2dxGLSurfaceView{
	
	public LuaGLSurfaceView(Context context){
		super(context);
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
    	// exit program when key back is entered
    	if (keyCode == KeyEvent.KEYCODE_BACK) {
    		android.os.Process.killProcess(android.os.Process.myPid());
    	}
        return super.onKeyDown(keyCode, event);
    }
}


