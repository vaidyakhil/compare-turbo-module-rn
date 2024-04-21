package com.rtncalculator;

import android.os.Handler;

import androidx.annotation.NonNull;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.bridge.WritableNativeMap;
import com.rtncalculator.NativeCalculatorSpec;

public class CalculatorModule extends NativeCalculatorSpec {

    public static String NAME = "RTNCalculator";
    

    CalculatorModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    @NonNull
    public String getName() {
        return NAME;
    }

    @Override
    public void add(double a, double b, Promise promise) {
        promise.resolve(a + b);
    }

    @Override
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