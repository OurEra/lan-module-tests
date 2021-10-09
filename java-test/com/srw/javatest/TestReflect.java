package com.srw.javatest;

import com.example.lib.java.FormatLog;
import java.lang.reflect.Method;
import java.lang.reflect.InvocationTargetException;

public class TestReflect extends BaseTest {

    class TestMethod {

        public void getRecord() {
            FormatLog.LogI("getRecord involked!");
        }
    }

    @Override
    void test() {
        TestMethod testmethod = new TestMethod();
        try {
            Method getrecord = testmethod.getClass().getDeclaredMethod("getRecord");
            getrecord.setAccessible(true);
            FormatLog.LogI("method " + getrecord);
            getrecord.invoke(testmethod);
        } catch (NoSuchMethodException | IllegalAccessException | InvocationTargetException e) {
            FormatLog.LogI("method error " + e);
        }
    }
}
