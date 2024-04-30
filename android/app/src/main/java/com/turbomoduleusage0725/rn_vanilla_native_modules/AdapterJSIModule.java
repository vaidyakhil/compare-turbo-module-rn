package com.turbomoduleusage0725.rn_vanilla_native_modules;

import android.util.Log;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.WritableMap;
import com.turbomoduleusage0725.JSONObjectUtil;
import com.turbomoduleusage0725.WritableMapUtils;

import org.json.JSONObject;

import androidx.annotation.NonNull;

public class AdapterJSIModule extends ReactContextBaseJavaModule {

    public AdapterJSIModule(ReactApplicationContext context) {
        super(context);
    }

    @NonNull
    @Override
    public String getName() {
        return "AdapterJSIModule";
    }

    @ReactMethod
    public void install(Promise promise) {
        try {
            System.loadLibrary("custom-jsi-module");

            JavaScriptContextHolder jsContext = getReactApplicationContext().getJavaScriptContextHolder();
             install_jsi_module_bindings(jsContext.get());

//            native_install();

            Log.d(getName(), "loading native library succeeded");
            promise.resolve(null);
        } catch (Exception exception) {
            Log.d(getName(), "loading native library failed with exception");
            promise.resolve(null);
        }
    }

    @ReactMethod
    public String get_string_from_jsi_via_jni() {
        return "Hello via custom JSI bindings via JNI";
    }

    @ReactMethod
    public WritableMap makeNetworkCall_get_writable_map () {
        WritableMap response = WritableMapUtils.getSampleWritableMap();
        return response;
    }

    @ReactMethod
    public JSONObject makeNetworkCall_get_jsonobject () {
        JSONObject response = JSONObjectUtil.getSampleJSONObject();
        return response;
    }


    private native void install_jsi_module_bindings (long jsiPointer);

    private native void native_install();
}
