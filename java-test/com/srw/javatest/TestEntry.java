package com.srw.javatest;

import com.example.lib.java.FormatLog;

public class TestEntry {

    private static final boolean TEST_BYTE_BUFFER = false;
    private static final boolean TEST_JSON        = false;
    private static final boolean TEST_STACK_TRACE = false;
    private static final boolean TEST_SYNC        = false;
    private static final boolean TEST_EQUAL       = false;
    private static final boolean TEST_ENUM        = true;
    private static final boolean TEST_DEQUE       = false;
    private static final boolean TEST_SEM         = false;
    private static final boolean TEST_BARR        = false;
    private static final boolean TEST_COMPARE     = false;
    private static final boolean TEST_EXEUTOR     = false;
    private static final boolean TEST_MAP         = false;
    private static final boolean TEST_FILE        = false;
    private static final boolean TEST_REFLECT     = true;
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
        if (TEST_DEQUE) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestBlockingDeque.class.getSimpleName());
            TestBlockingDeque test = new TestBlockingDeque();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestBlockingDeque.class.getSimpleName());
        }
        if (TEST_SEM) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestSemphore.class.getSimpleName());
            TestSemphore test = new TestSemphore();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestSemphore.class.getSimpleName());
        }
        if (TEST_BARR) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestBarrier.class.getSimpleName());
            TestBarrier test = new TestBarrier();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestBarrier.class.getSimpleName());
        }
        if (TEST_COMPARE) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestCompare.class.getSimpleName());
            TestCompare test = new TestCompare();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestCompare.class.getSimpleName());
        }
        if (TEST_EXEUTOR) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestExeutor.class.getSimpleName());
            TestExeutor test = new TestExeutor();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestExeutor.class.getSimpleName());
        }
        if (TEST_MAP) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestMap.class.getSimpleName());
            TestMap test = new TestMap();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestMap.class.getSimpleName());
        }
        if (TEST_FILE) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestRandomAccessFile.class.getSimpleName());
            TestRandomAccessFile test = new TestRandomAccessFile();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestRandomAccessFile.class.getSimpleName());
        }
        if (TEST_REFLECT) {
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST BEGIN " + TestReflect.class.getSimpleName());
            TestReflect test = new TestReflect();
            test.test();
            FormatLog.LogI(FormatLog.ANSI_RED, "TEST END " + TestReflect.class.getSimpleName());
        }
    }
}
