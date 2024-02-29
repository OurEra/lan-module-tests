package com.srw.demo_server;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.HandlerThread;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

import com.android.internal.util.AsyncChannel;

public class ServiceTest extends Service {

    private static final String TAG = "srwTestServer";
    private static final String TRIGGER_START_SERVICE = "com.srw.demo_app.action.TRIGGER_START_SERVICE";

    private ServiceTestImpl mServiceImpl;

    private Looper mLooper;
    private MyHandler mHandler;
    private AsyncChannel mAChannel;

    public ServiceTest() {

        mServiceImpl = new ServiceTestImpl();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mServiceImpl;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.d(TAG, "onUnbind received");
        return false;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "ServiceTest onCreate");

        HandlerThread thread = new HandlerThread("ServiceTest");
        thread.start();

        mLooper = thread.getLooper();
        mHandler = new MyHandler(mLooper);
    }

    private class ServiceTestImpl extends IServiceTest.Stub {
        @Override
        public Messenger getTestMessenger() {
            return new Messenger(mHandler);
        }
    }

     private final class MyHandler extends Handler {
         public MyHandler(Looper looper) {
             super(looper);
         }

         @Override
         public void handleMessage(Message msg) {

            switch (msg.what) {

                case AsyncChannel.CMD_CHANNEL_FULL_CONNECTION: {
                    Log.d(TAG, "CMD_CHANNEL_FULL_CONNECTION");

                    // reply directly but didn't save msg.replyTo for future usage,
                    // there also haven't our own 'replyTo' to pass to the other side
                    /*
                    try {
                        Message reply = new Message();
                        reply.what = AsyncChannel.CMD_CHANNEL_FULLY_CONNECTED;
                        msg.replyTo.send(reply);
                    } catch (RemoteException e) {
                        Log.e(TAG, "reply messenge e " + e.toString());
                    }
                    */

                     AsyncChannel ac = new AsyncChannel();
                     Log.d(TAG, "Instance AsyncChannel " + ac);
                     ac.connected(null, this, msg.replyTo);
                     ac.replyToMessage(msg, AsyncChannel.CMD_CHANNEL_FULLY_CONNECTED,
                             AsyncChannel.STATUS_SUCCESSFUL);

                     mAChannel = ac;
                }
                break;

                case AsyncChannel.CMD_CHANNEL_DISCONNECTED: {

                    Log.d(TAG, "CMD_CHANNEL_DISCONNECTED from " + msg.obj);
                }
                break;
            }
         }
     }
}
