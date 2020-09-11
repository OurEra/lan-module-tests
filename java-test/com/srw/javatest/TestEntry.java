package com.srw.javatest;

import com.example.lib.java.FormatLog;


public class TestEntry {

    private static final boolean TEST_BYTE_BUFFER = true;
    private static final boolean TEST_JSON        = false;
    private static final boolean TEST_STACK_TRACE = false;
    private static final boolean TEST_SYNC        = false;
    private static final boolean TEST_EQUAL       = false;
    private static final boolean TEST_ENUM        = true;
    public static void main(String[] args) {

        if (TEST_BYTE_BUFFER) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestByteBuffer.class.getSimpleName());
            TestByteBuffer testByteBuffer = new TestByteBuffer();
            testByteBuffer.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestByteBuffer.class.getSimpleName());
        }
        if (TEST_STACK_TRACE) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestStackTrace.class.getSimpleName());
            TestStackTrace testStack = new TestStackTrace();
            testStack.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestStackTrace.class.getSimpleName());
        }
        if (TEST_JSON) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestJSON.class.getSimpleName());
            TestJSON testJSON = new TestJSON();
            testJSON.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestJSON.class.getSimpleName());
        }
        if (TEST_SYNC) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestSync.class.getSimpleName());
            TestSync testSync = new TestSync();
            testSync.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestSync.class.getSimpleName());
        }
        if (TEST_EQUAL) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestEquals.class.getSimpleName());
            TestEquals testEquals = new TestEquals();
            testEquals.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestEquals.class.getSimpleName());
        }
        if (TEST_ENUM) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestEnum.class.getSimpleName());
            TestEnum testEnum = new TestEnum();
            testEnum.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestEnum.class.getSimpleName());
        }
    }
}
