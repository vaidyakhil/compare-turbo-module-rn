package com.turbomoduleusage0725.rn_vanilla_native_modules;

import com.facebook.react.ReactPackage;
import com.facebook.react.bridge.NativeModule;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ViewManager;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class CustomReactPackage implements ReactPackage {
	public CustomReactPackage() {
	}

	/**
	 * @param reactContext react application context that can be used to create modules
	 * @return list of native modules to register with the newly created catalyst instance
	 */
	// @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP_MR1)
	@Override
	public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
		List<NativeModule> modules = new ArrayList<>();
		modules.add(new OldCalculatorModule(reactContext));
		modules.add(new AdapterJSIModule(reactContext));
        return modules;
	}

	@Override
	public List<ViewManager> createViewManagers(ReactApplicationContext reactContext) {
		return Collections.emptyList();
	}
}

