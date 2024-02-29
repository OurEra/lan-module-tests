package com.srw.demo_app;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;

import android.net.ConnectivityManager;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiConfiguration.AuthAlgorithm;
import android.net.wifi.WifiConfiguration.KeyMgmt;
import android.net.wifi.ScanResult;

import com.kedacom.controlsdk.ControlSdkService;
import com.kedacom.controlsdk.NetConfig;
import com.srw.demo_app.R;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.util.Properties;
import java.util.List;


// TAG: srwTest ControlService EthernetServiceImpl
// /data/misc/ethernet/ipconfig.txt
// out/target/product/kdm2412mx/obj/JAVA_LIBRARIES/controlsdk_intermediates/javalib.jar
// java -jar ./prebuilts/sdk/tools/lib/signapk.jar ./vendor/kedacom/security/platform.x509.pem ./vendor/kedacom/security/platform.pk8 ~/AndroidStudio/controlsdktest/app/build/outputs/apk/debug/app-debug.apk out.apk
// am start -n com.srw.demo_app/.ControlTest

// route list
// default via 172.16.124.77 dev eth0  proto static
// 172.16.120.0/21 dev eth0  proto kernel  scope link  src 172.16.124.235
public class ControlTest extends Activity {

    private static final String TAG = "srwTestApp";
    private ControlSdkService mSdkService;

    private WifiManager mWifiManager;
    private ConnectivityManager mCm;

    private static final int TEST_CMD_ETH			= 0x10000;
    private static final int TEST_CMD_ETH_GET		= 0x10001;
    private static final int TEST_CMD_WIFI_AP		= 0x10002;
	private static final int TEST_CMD_WIFI_STA      = 0x10003;
    private static final int TEST_CMD_WF_SCAN_RET   = 0x10004;
    private static final int TEST_CMD_MOBILE		= 0x10005;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.dummy_lay);

        mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        mCm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);

        mSdkService = new ControlSdkService(getApplicationContext(), new ControlSdkService.CallBack() {
            @Override
            public void serviceConnected(ControlSdkService controlSdkService) {

                Log.d(TAG, "control sdk callback");
                Message msg1 = mTestHan.obtainMessage();
                msg1.what = TEST_CMD_WIFI_STA;
                msg1.arg1 = 0;
                msg1.sendToTarget();

                //Message msg2 = mTestHan.obtainMessage();
                //msg2.what = TEST_CMD_MOBILE;
                //msg2.arg1 = 1;
                //mTestHan.sendMessageDelayed(msg2, 5000);
            }
        });

		IntentFilter filter = new IntentFilter();
		filter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
		filter.addAction(WifiManager.WIFI_SCAN_AVAILABLE);
		registerReceiver(
		        new BroadcastReceiver() {
		            @Override
		            public void onReceive(Context context, Intent intent) {
		                String action = intent.getAction();
		
						Log.d(TAG, "onReceive " + action);
		                if (action.equals(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)) {
                			Message msg1 = mTestHan.obtainMessage();
                			msg1.what = TEST_CMD_WF_SCAN_RET;
                			msg1.sendToTarget();
		                } else if (action.equals(WifiManager.WIFI_SCAN_AVAILABLE)) {

							mWifiManager.startScan();
						}
		            }
		        }, filter);

    }

    public ControlSdkService getService() {

        return mSdkService;
    }

    public Handler getHandler() {

        return mTestHan;
    }

    private Handler mTestHan = new MyHandler(this);
    private static class MyHandler extends Handler {
        private final WeakReference<Activity> mActivity;
        public MyHandler(Activity activity) {
            mActivity = new WeakReference<Activity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {

            if(mActivity.get() == null) {
                return;
            }

            switch (msg.what) {

                case TEST_CMD_ETH: {

                    ((ControlTest)mActivity.get()).testEthernet();
                }
                break;

                case TEST_CMD_ETH_GET: {

                    int now = ((ControlTest)mActivity.get()).getService().getEthState(0);
                    Log.d(TAG, "get state " + now);
                    NetConfig config = ((ControlTest)mActivity.get()).getService().getEthConfiguration(0);

                    Log.d(TAG, "get now conf " + config.toString());
                }
                break;

                case TEST_CMD_WIFI_AP: {

                    ((ControlTest)mActivity.get()).testWifiAp();
                }
                break;

				case TEST_CMD_WIFI_STA: {

                    ((ControlTest)mActivity.get()).testWifiSta();
				}
                break;

				case TEST_CMD_WF_SCAN_RET: {

                    ((ControlTest)mActivity.get()).getWifiScan();
				}
                break;

                case TEST_CMD_MOBILE: {

                    ((ControlTest)mActivity.get()).testEnableMobileData(msg.arg1);
                }
                break;

            }

        }
    }

    public void testEthernet() {

        // state:on
        // static:true
        // address:192.168.0.22
        // mask:255.255.255.0
        // gateway:192.168.0.1
        // dns1:8.8.8.8
        Properties properties = new Properties();
        try {
            FileInputStream fin = new FileInputStream("/sdcard/net.conf");
            properties.load(fin);
            fin.close();
        }catch (IOException e){
            Log.d(TAG, "property err "+ e.toString());
            return;
        }

        String state = properties.getProperty("state");
        String isstatic = properties.getProperty("static");
        String address = properties.getProperty("address");
        String mask = properties.getProperty("mask");
        String gateway = properties.getProperty("gateway");
        String dns1 = properties.getProperty("dns1");
        String dns2 = properties.getProperty("dns2");
        Log.d(TAG, "isstatic " + isstatic +
                    " address " + address +
                    " mask " + mask +
                    " gateway " + gateway +
                    " dns1 " + dns1 +
                    " dns2 " + dns2);

        int old = mSdkService.getEthState(0);
        Log.d(TAG, "get old state " + old);
        NetConfig oldconfig = mSdkService.getEthConfiguration(0);

        Log.d(TAG, "get old conf " + oldconfig.toString());

        mSdkService.setEthState(0, state.equals("on") ? 1 : 0);
        NetConfig config = new NetConfig(isstatic.equals("true"), address, mask, gateway, dns1, dns2);
        mSdkService.setEthConfiguration(0, config);

        mTestHan.sendMessageDelayed(mTestHan.obtainMessage(TEST_CMD_ETH_GET), 3000);
    }

	public void testWifiSta() {

		mWifiManager.setWifiEnabled(true);
	}

	public void getWifiScan() {

		List<ScanResult> ret = mWifiManager.getScanResults();
		Log.d(TAG, "get Scan size " + ret.size() + " One is: " + ret.get(0));
	}

    public void testWifiAp() {

        Log.d(TAG, "testWifiAp...");
        mWifiManager.setWifiApEnabled(null, false);

        WifiConfiguration config = mWifiManager.getWifiApConfiguration();
        config.allowedKeyManagement.set(KeyMgmt.WPA2_PSK);
        config.allowedAuthAlgorithms.set(AuthAlgorithm.OPEN);
        config.preSharedKey = "66666666";
        mWifiManager.setWifiApConfiguration(config);

        mWifiManager.setWifiApEnabled(null, true);
        mCm.startTethering(ConnectivityManager.TETHERING_WIFI, true, mStartTetheringCallback, mTestHan);
    }

    public void testEnableMobileData(int arg) {

        Log.d(TAG, "testEnableMobileData: " + arg);
        mSdkService.setMobileDataEnabled(arg > 0 ? true : false);
    }

    private OnStartTetheringCallback mStartTetheringCallback = new OnStartTetheringCallback();
    private static final class OnStartTetheringCallback extends
            ConnectivityManager.OnStartTetheringCallback {

        @Override
        public void onTetheringStarted() {
        }

        @Override
        public void onTetheringFailed() {
        }
    }

}
