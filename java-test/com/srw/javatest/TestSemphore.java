package com.srw.javatest;

import com.example.lib.java.FormatLog;
import java.util.concurrent.Semaphore;

public class TestSemphore extends BaseTest {

    Semaphore mSem = new Semaphore(0);

    @Override
    void test() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    mSem.acquire();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                FormatLog.LogI("run after acquire " + mSem.availablePermits());
            }
        }).start();

        FormatLog.LogI("sem " + mSem.availablePermits() + " before release");
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        mSem.release();
        FormatLog.LogI("sem " + mSem.availablePermits() + " after release");
    }

}
