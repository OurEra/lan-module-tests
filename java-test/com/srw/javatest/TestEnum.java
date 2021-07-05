package com.srw.javatest;

import org.apache.commons.codec.binary.Hex;
import com.example.lib.java.FormatLog;

public class TestEnum extends BaseTest {

    enum SimpleEnum {
        V1("v-1"),
        V2("v-2"),
        V3("vv3");

        private static SimpleEnum[] mValues = values();

        private final String mValue;
        SimpleEnum(String v) {
            mValue = v;
        }
        public String getValue() {
            return mValue;
        }
        public SimpleEnum[] getValues() {
            return mValues;
        }
    }

    @Override
    void test() {
        String str = "V1";
        SimpleEnum simpleEnum = SimpleEnum.valueOf(str);
        FormatLog.LogI("enum " + simpleEnum.mValue);

        SimpleEnum[] vals = simpleEnum.getValues();
        for (SimpleEnum v : vals)
            FormatLog.LogI("value " + v.getValue());


        String format = "%s %d %d %s";
        String s1 = "asfsasaf";
        int d2 = 5000;
        int d3 = 5000;
        String s4 = "I";
        FormatLog.LogI("TEST:" + String.format(format, s1, d2, d3, s4));

        byte[] bytes = new byte[5];
        bytes[0] = 0x01;
        bytes[1] = 0x02;
        bytes[2] = 0x11;
        bytes[3] = 0x31;
        bytes[4] = 0x41;

        String hexStr = Hex.encodeHexString(bytes);
        FormatLog.LogI("TEST:" + hexStr + "; len " + hexStr.length());
    }
}
