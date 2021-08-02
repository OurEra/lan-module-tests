package com.srw.javatest;

import com.example.lib.java.FormatLog;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class TestRandomAccessFile extends BaseTest {

    private RandomAccessFile mRandomFile;

    @Override
    void test() {
        // delete old one
        new File("testRandomFile.txt").delete();
        try {
            mRandomFile = new RandomAccessFile(new File("testRandomFile.txt"), "rw");

            String writeInfo = "line1\n";
            String writeInfo2 = "line2\n";
            String writeInfo3 = "line3\n";
            mRandomFile.write(writeInfo.getBytes());
            mRandomFile.write(writeInfo2.getBytes());
            mRandomFile.write(writeInfo3.getBytes());

            FormatLog.LogI("After write pos:" + mRandomFile.getFilePointer() + ", length " + mRandomFile.length());
            mRandomFile.setLength(mRandomFile.length() - writeInfo3.getBytes().length);
            FormatLog.LogI("After set length pos:" + mRandomFile.getFilePointer() + ", length " + mRandomFile.length());

            mRandomFile.seek(0);
            String read = mRandomFile.readLine();
            FormatLog.LogI("After read pos:" + mRandomFile.getFilePointer() + ", read:" + read);

        } catch (IOException e) {
            FormatLog.LogI("File error:" + e.getMessage());
        }
    }
}
