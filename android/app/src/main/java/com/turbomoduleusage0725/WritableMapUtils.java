package com.turbomoduleusage0725;

import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.bridge.WritableNativeArray;
import com.facebook.react.bridge.WritableNativeMap;

public class WritableMapUtils {

    public static boolean isPrimitiveValue(Object obj) {
        return obj instanceof Boolean || obj instanceof Integer || obj instanceof Double || obj instanceof Float || obj instanceof Long || obj instanceof Character || obj instanceof Short || obj instanceof Byte;
    }

    public static WritableMap getSampleWritableMap() {
        WritableMap resMap = new WritableNativeMap();

        WritableArray pledgeables = new WritableNativeArray();
        for (int i=0; i<200; i++) {
            WritableMap pledgeable = new WritableNativeMap();
            pledgeable.putString("df" + Math.random(), "sdfsfds");
            pledgeable.putInt("xvsd", 2);
            pledgeable.putInt("bfsdg", 2);
            pledgeable.putInt("xdasfd" + Math.random(), 3340);
            pledgeable.putInt("cvefs", 2404);
            pledgeable.putInt("eafef", 28);

            WritableMap companyInfo = new WritableNativeMap();
            companyInfo.putString("rgdfgvxc" + Math.random(), "MMTC");
            companyInfo.putString("adfsdc", "MMTC");
            companyInfo.putString("wsdfcd" + Math.random(), "513377");
            companyInfo.putString("xcvsrfb", "mmtc-ltd");
            pledgeable.putMap("asddc" + Math.random(), companyInfo);

            pledgeables.pushMap(pledgeable);
        }

        resMap.putArray("pledgables", pledgeables);
        return resMap;
    }
}
