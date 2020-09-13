package com.example.lib.java;

import java.text.SimpleDateFormat;
import java.util.Date;

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

    public static void LogI(String COLOR , String msg, int offset ) {
        long tid = Thread.currentThread().getId();
        StackTraceElement[] trace = Thread.currentThread().getStackTrace();
        Date now = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
        String timestamp = sdf.format(now);
        String format = timestamp + " [" +
                trace[offset].getFileName() + "(" + tid + ")"
                + " "
                + trace[offset].getMethodName() + ":" + trace[offset].getLineNumber()
                + "] ";
        if (mCallback != null) {
            mCallback.onLog(format + msg);
        } else {
            System.out.println(COLOR + format + msg);
        }
    }

    public static void LogI(String COLOR , String msg) {
        LogI(COLOR, msg, OFFSET + 1);
    }

    public static void LogI(String msg) {
        LogI(ANSI_CYAN, msg, OFFSET + 1);
    }
}
