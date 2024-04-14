package com.turbomoduleusage0725.rn_vanilla_native_modules;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.Promise;
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
    public void add(double a, double b, Promise promise) {
        promise.resolve(a + b);
    }
}
