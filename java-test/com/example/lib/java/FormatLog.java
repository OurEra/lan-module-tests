package com.example.lib.java;

public class FormatLog {

    private static final int OFFSET = 2;
    private static FormatLogCallback mCallback;

    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_BLACK = "\u001B[30m";
    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_YELLOW = "\u001B[33m";
    public static final String ANSI_BLUE = "\u001B[34m";
    public static final String ANSI_PURPLE = "\u001B[35m";
    public static final String ANSI_CYAN = "\u001B[36m";
    public static final String ANSI_WHITE = "\u001B[37m";

    public interface FormatLogCallback {
        void onLog(String log);
    }


    public static void LogI(String COLOR , String msg) {
        long tid = Thread.currentThread().getId();
        StackTraceElement[] trace = Thread.currentThread().getStackTrace();
        String format = "[" +
                trace[OFFSET].getFileName() + "(" + tid + ")"
                + " "
                + trace[OFFSET].getMethodName() + ":" + trace[OFFSET].getLineNumber()
                + "]";
        if (mCallback != null) {
            mCallback.onLog(format + msg);
        } else {
            System.out.println(COLOR + format + msg);
        }
    }

    public static void LogI(String msg) {
        LogI(ANSI_CYAN, msg);
    }
}
