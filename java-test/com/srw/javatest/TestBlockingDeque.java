package com.srw.javatest;

import com.example.lib.java.FormatLog;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.TimeUnit;

// https://docs.oracle.com/javase/8/docs/api/java/util/concurrent/LinkedBlockingDeque.html
public class TestBlockingDeque extends BaseTest {

    void pause() {
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }


    @Override
    void test() {
        LinkedBlockingDeque<String> instance = new LinkedBlockingDeque<String>(5);
        Thread tQueue = new Thread(new Runnable() {
             @Override
             public void run() {
				 
				pause();
				instance.offer("111");
                FormatLog.LogI("offered 111 size " + instance.size());

				pause();
				instance.offer("222");
                FormatLog.LogI("offered 222 size " + instance.size());

				pause();
				instance.offer("333");
                FormatLog.LogI("offered 333 size " + instance.size());

				pause();
				instance.offer("444");
                FormatLog.LogI("offered 444 size " + instance.size());

				pause();
				instance.offer("555");
                FormatLog.LogI("offered 555 size " + instance.size());

				pause();
				instance.offer("666");
                FormatLog.LogI("offered 666 size " + instance.size());
             }
         });

        Thread tDeque = new Thread(new Runnable() {
             @Override
             public void run() {
				try {
					String ret = instance.poll(500, TimeUnit.MILLISECONDS);
                	FormatLog.LogI("poll " + ret + " size " + instance.size());

					ret = instance.poll(500, TimeUnit.MILLISECONDS);
                	FormatLog.LogI("poll " + ret + " size " + instance.size());

					ret = instance.poll(500, TimeUnit.MILLISECONDS);
                	FormatLog.LogI("poll " + ret + " size " + instance.size());

					ret = instance.poll(500, TimeUnit.MILLISECONDS);
                	FormatLog.LogI("poll " + ret + " size " + instance.size());

					ret = instance.poll(500, TimeUnit.MILLISECONDS);
                	FormatLog.LogI("poll " + ret + " size " + instance.size());

					ret = instance.poll(500, TimeUnit.MILLISECONDS);
                	FormatLog.LogI("poll " + ret + " size " + instance.size());
				} catch (InterruptedException e) {
				}
             }
         });

		tDeque.start();
		tQueue.start();
		try {
			tDeque.join();
			tQueue.join();
		} catch (InterruptedException e) {
		}
        FormatLog.LogI("dequeu over size " + instance.size());
    }
}
