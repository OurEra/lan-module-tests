package com.srw.javatest;

import com.example.lib.java.FormatLog;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.BrokenBarrierException;

public class TestBarrier extends BaseTest {

    int SYNC_NUM = 3;
    int index = 0;

    CyclicBarrier barrier = new CyclicBarrier(SYNC_NUM);

    @Override
    void test() {
        for (; index < SYNC_NUM; index++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    FormatLog.LogI("before await");
                    try {
                        Thread.sleep(index > 0 ? (index *1000) : 1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    try {
                        barrier.await();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } catch(BrokenBarrierException e) {
                        e.printStackTrace();
                    }
                    FormatLog.LogI("after await");
                }
            }).start();
        }

        try {
            Thread.sleep(SYNC_NUM * SYNC_NUM * 1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

}
