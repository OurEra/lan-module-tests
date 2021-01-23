package com.srw.javatest;

import com.example.lib.java.FormatLog;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class TestExeutor extends BaseTest {

    private ScheduledExecutorService mExecutor;

    @Override
    void test() {

        mExecutor = Executors.newSingleThreadScheduledExecutor();

        mExecutor.schedule(new Runnable() {
            @Override
            public void run() {
                FormatLog.LogI("run after shutdown");
            }
        }, 5, TimeUnit.SECONDS);

        mExecutor.shutdown();
    }
}
