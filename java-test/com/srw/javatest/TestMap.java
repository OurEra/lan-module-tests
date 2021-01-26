package com.srw.javatest;

import com.example.lib.java.FormatLog;
import java.lang.Thread;
import java.lang.InterruptedException;
import java.util.Map;
import java.util.HashMap;
import java.util.concurrent.CountDownLatch;

public class TestMap extends BaseTest {

    private Map<String, String> mMap = new HashMap<>();
    private final Object mLockMap = new Object();
    private CountDownLatch mLatch = new CountDownLatch(2);

    @Override
    void test() {

        new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < 100; i++) {
                    synchronized(mLockMap) {
                        for (String value : mMap.values()) {
                            FormatLog.LogI("round " + i + " value " + value);
                        }
                    }
                    try {
                        Thread.sleep(2);
                    } catch (InterruptedException e) {}
                }

                mLatch.countDown();
            }
        }).start();

        new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < 100; i++) {
                    synchronized(mLockMap) {
                        mMap.put("base" + i, "content" + i);
                    }
                    try {
                        Thread.sleep(2);
                    } catch (InterruptedException e) {}
                }
                mLatch.countDown();
            }
        }).start();

        try {
            mLatch.await();
        } catch (InterruptedException e) {}

    }
}
