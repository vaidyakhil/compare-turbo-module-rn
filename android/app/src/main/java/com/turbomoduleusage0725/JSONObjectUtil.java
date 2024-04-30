package com.turbomoduleusage0725;

import android.util.Log;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class JSONObjectUtil {
    public static JSONObject getSampleJSONObject() {
        try {
            JSONObject resObj = new JSONObject();

            JSONArray pledgeables = new JSONArray();
            for (int i=0; i<200; i++) {
                JSONObject pledgeable = new JSONObject();
                pledgeable.put("df", "sdfsfds");
                pledgeable.put("xvsd", 2);
                pledgeable.put("bfsdg", 2);
                pledgeable.put("xdasfd" + Math.random(), 3340);
                pledgeable.put("cvefs", 2404);
                pledgeable.put("eafef", 28);

                JSONObject companyInfo = new JSONObject();
                companyInfo.put("rgdfgvxc" + Math.random(), "MMTC");
                companyInfo.put("adfsdc", "MMTC");
                companyInfo.put("wsdfcd" + Math.random(), "513377");
                companyInfo.put("xcvsrfb", "mmtc-ltd");
                pledgeable.put("asddc" + Math.random(), companyInfo);

                pledgeables.put(pledgeable);
            }

            resObj.put("pledgables", pledgeables);
            return resObj;
        } catch (JSONException exception) {
            Log.d("custom-jsi-module", "Exception aaya re :((");
            return new JSONObject();
        }
    }
}
