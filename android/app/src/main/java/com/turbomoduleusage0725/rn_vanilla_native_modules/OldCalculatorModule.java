package com.turbomoduleusage0725.rn_vanilla_native_modules;

import android.os.Handler;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.turbomoduleusage0725.WritableMapUtils;

import androidx.annotation.NonNull;

public class OldCalculatorModule extends ReactContextBaseJavaModule {

    public static String NAME = "OldCalculatorModule";

    public OldCalculatorModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    @NonNull
    public String getName() {
        return NAME;
    }

    @ReactMethod
    public String get_string_via_bridge() {
        return "Hello via bridge";
    }

    @ReactMethod
    public void makeNetworkCall(ReadableMap requestBody, Promise promise) {
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                WritableMap response = WritableMapUtils.getSampleWritableMap();
                promise.resolve(response);
            }
        }, 1200);
    }
}
