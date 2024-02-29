package com.srw.demo_app;

import android.os.HandlerThread;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.content.Context;
import android.content.ServiceConnection;
import android.content.ComponentName;
import com.android.internal.util.AsyncChannel;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

import com.srw.demo_server.IServiceTest;

// am startservice com.srw.demo_app/.MiscTest
public class MiscTest extends Service {

    private static final String TAG = "srwTestApp";
    private static final String TRIGGER_START_SERVICE = "com.srw.demo_app.action.TRIGGER_START_SERVICE";

    private static final int TEST_CMD_SM            = 0x10000;
    private static final int TEST_CMD_BROAD         = 0x10001;
    private static final int TEST_CMD_BIND_SER      = 0x10002;
    private static final int TEST_CMD_UNBIND_SER    = 0x10003;
    private static final int TEST_CMD_DIS_ACHANNEL  = 0x10004;

    private TestServiceConnection mConn;
    private IServiceTest mTestService;
    private AsyncChannel mAChannel;

    private Looper mServiceLooper;
    private ServiceHandler mServiceHandler;

    StateMachineTest mSMTest;

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "MiscTest onCreate");

        HandlerThread thread = new HandlerThread("MiscTestService");
        thread.start();
        mServiceLooper = thread.getLooper();
        mServiceHandler = new ServiceHandler(mServiceLooper);

        mSMTest = StateMachineTest.makeSM();

        mConn = new TestServiceConnection();

        Message msg = mServiceHandler.obtainMessage();
        msg.what = TEST_CMD_SM;
        msg.arg1 = 1;
        mServiceHandler.sendMessage(msg);

        Message msg2 = mServiceHandler.obtainMessage();
        msg2.what = TEST_CMD_SM;
        msg2.arg1 = 0;
        mServiceHandler.sendMessageDelayed(msg2, 5000);

        Message msg3 = mServiceHandler.obtainMessage();
        msg3.what = TEST_CMD_BIND_SER;
        mServiceHandler.sendMessageDelayed(msg3, 0);

        Message msg4 = mServiceHandler.obtainMessage();
        msg4.what = TEST_CMD_UNBIND_SER;
        mServiceHandler.sendMessageDelayed(msg4, 6000);
    }

     private class TestServiceConnection implements ServiceConnection {

         @Override
         public void onServiceConnected(ComponentName name, IBinder service) {
            mTestService = IServiceTest.Stub.asInterface(service);
            Log.d(TAG, "Connected to service: " + name.flattenToString());
            try {
                Messenger messenger = mTestService.getTestMessenger();
                Log.d(TAG, "Get Messenger " + messenger);

                mAChannel = new AsyncChannel();
                Log.d(TAG, "Instance AsyncChannel " + mAChannel);
                mAChannel.connect(MiscTest.this, mServiceHandler, messenger);

                Message msg = mServiceHandler.obtainMessage();
                msg.what = TEST_CMD_DIS_ACHANNEL;
                mServiceHandler.sendMessageDelayed(msg, 4000);

            } catch (RemoteException e) {
                Log.e(TAG, "get messenger e " + e.toString());
            }
         }

         @Override
         public void onServiceDisconnected(ComponentName name) {
             Log.d(TAG, "Disconnected from servie: " + name.flattenToString());
             mTestService = null;
         }
     }

     private final class ServiceHandler extends Handler {
         public ServiceHandler(Looper looper) {
             super(looper);
         }

         @Override
         public void handleMessage(Message msg) {

            switch (msg.what) {

                case TEST_CMD_SM: {

                    if (msg.arg1 > 0)
                        mSMTest.start();
                    else
                        mSMTest.destory();
                }
                break;

                case TEST_CMD_BROAD: {

                    Intent intent = new Intent(TRIGGER_START_SERVICE);
                    sendBroadcast(intent);
                }
                break;

                case TEST_CMD_BIND_SER: {

                    // bind to IServiceTest
                    Intent testservice = new Intent("com.srw.demo_server.ServiceTest");
                    testservice.setPackage("com.srw.demo_server");
                    try {
                        boolean ret = MiscTest.this.bindService(testservice, mConn,
                                                Context.BIND_AUTO_CREATE | Context.BIND_FOREGROUND_SERVICE);
                    } catch (SecurityException ex) {
                        Log.e(TAG, "bind service e: " + ex.toString());
                    }
                }
                break;

                case TEST_CMD_UNBIND_SER: {

                    Log.d(TAG, "do TEST_CMD_UNBIND_SER");
                    MiscTest.this.unbindService(mConn);
                }
                break;

                case TEST_CMD_DIS_ACHANNEL: {
                    mAChannel.disconnect();
                }
                break;

                case AsyncChannel.CMD_CHANNEL_HALF_CONNECTED: {
                    if (msg.arg1 == AsyncChannel.STATUS_SUCCESSFUL) {
                        Log.d(TAG, "CMD_CHANNEL_HALF_CONNECTED from " + msg.obj);
                        mAChannel.sendMessage(AsyncChannel.CMD_CHANNEL_FULL_CONNECTION);
                    } else {
                        Log.e(TAG, "Failed to set up channel connection");
                        mAChannel = null;
                    }
                }
                break;

                case AsyncChannel.CMD_CHANNEL_FULLY_CONNECTED: {

                    Log.d(TAG, "CMD_CHANNEL_FULLY_CONNECTED");
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
