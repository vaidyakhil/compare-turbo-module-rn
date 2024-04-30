#include <iostream>
#include <jni.h>
#include <android/log.h>
#include <jsi/jsi.h>
#include "pthread.h"
#include <sys/types.h>
#include "thread"
#include <chrono>
#include <fbjni/fbjni.h>

using namespace facebook::jsi;
using namespace facebook::jni;
using namespace std;

JavaVM *java_vm;

JNIEnv *jniEnvTest;

/**
* A simple callback function that allows us to detach current JNI Environment
* when the thread
* See https://stackoverflow.com/a/30026231 for detailed explanation
*/


facebook::jsi::Value convertJNIObjectToJSIValue(
        facebook::jsi::Runtime& runtime,
        const facebook::jni::local_ref<jobject>& object
) {
    if (object == nullptr) {
        // null

        return facebook:: jsi::Value::undefined();
    } else if (object->isInstanceOf(facebook::jni::JBoolean::javaClassStatic())) {
        // Boolean

        auto boxed = static_ref_cast<facebook::jni::JBoolean>(object);
        bool value = boxed->value();
        return facebook:: jsi::Value(value);
    } else if (object->isInstanceOf( facebook:: jni::JDouble::javaClassStatic())) {
        // Double

        auto boxed = static_ref_cast<JDouble>(object);
        double value = boxed->value();
        return facebook:: jsi::Value(value);
    } else if (object->isInstanceOf( facebook:: jni::JInteger::javaClassStatic())) {
        // Integer

        auto boxed = static_ref_cast<JInteger>(object);
        return facebook:: jsi::Value(boxed->value());
    } else if (object->isInstanceOf( facebook:: jni::JString::javaClassStatic())) {
        // String

        return facebook:: jsi::String::createFromUtf8(runtime, object->toString());
    } else if (object->isInstanceOf(JList<jobject>::javaClassStatic())) {
        // List<E>

        auto arrayList = static_ref_cast<JList<jobject>>(object);
        auto size = arrayList->size();

        auto result = facebook:: jsi::Array(runtime, size);
        size_t i = 0;
        for (const auto& item : *arrayList) {
            result.setValueAtIndex(runtime, i, convertJNIObjectToJSIValue(runtime, item));
            i++;
        }
        return result;
    } else if (object->isInstanceOf(JMap<jstring, jobject>::javaClassStatic())) {
        // Map<K, V>

        auto map = static_ref_cast<JMap<jstring, jobject>>(object);

        auto result = facebook:: jsi::Object(runtime);
        for (const auto& entry : *map) {
            auto key = entry.first->toString();
            auto value = entry.second;
            auto jsiValue = convertJNIObjectToJSIValue(runtime, value);
            result.setProperty(runtime, key.c_str(), jsiValue);
        }
        return result;
    }

    auto type = object->getClass()->toString();
    auto message = "Cannot convert Java type \"" + type + "\" to facebook:: jsi::Value!";
    __android_log_write(ANDROID_LOG_ERROR, "VisionCamera", message.c_str());
    throw std::runtime_error(message);
}

void DeferThreadDetach(JNIEnv *env) {
    static pthread_key_t thread_key;

    // Set up a Thread Specific Data key, and a callback that
    // will be executed when a thread is destroyed.
    // This is only done once, across all threads, and the value
    // associated with the key for any given thread will initially
    // be NULL.
    static auto run_once = [] {
        const auto err = pthread_key_create(&thread_key, [](void *ts_env) {
            if (ts_env) {
                java_vm->DetachCurrentThread();
            }
        });
        if (err) {
            // Failed to create TSD key. Throw an exception if you want to.
        }
        return 0;
    }();

    // For the callback to actually be executed when a thread exits
    // we need to associate a non-NULL value with the key on that thread.
    // We can use the JNIEnv* as that value.
    const auto ts_env = pthread_getspecific(thread_key);
    if (!ts_env) {
        if (pthread_setspecific(thread_key, env)) {
            // Failed to set thread-specific value for key. Throw an exception if you
            // want to.
        }
    }
}

/**
* Get a JNIEnv* valid for this thread, regardless of whether
* we're on a native thread or a Java thread.
* If the calling thread is not currently attached to the JVM
* it will be attached, and then automatically detached when the
* thread is destroyed.
*
* See https://stackoverflow.com/a/30026231 for detailed explanation
*/
JNIEnv *GetJniEnv() {
    JNIEnv *env = nullptr;
    // We still call GetEnv first to detect if the thread already
    // is attached. This is done to avoid setting up a DetachCurrentThread
    // call on a Java thread.

    // g_vm is a global.
    auto get_env_result = java_vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (get_env_result == JNI_EDETACHED) {
        if (java_vm->AttachCurrentThread(&env, NULL) == JNI_OK) {
            DeferThreadDetach(env);
        } else {
            // Failed to attach thread. Throw an exception if you want to.
        }
    } else if (get_env_result == JNI_EVERSION) {
        // Unsupported JNI version. Throw an exception if you want to.
    }
    return env;
}

jobject jAdapterJsiModuleObject;

void say_hello_world()
{
    __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "hello world");

    return;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_turbomoduleusage0725_rn_1vanilla_1native_1modules_AdapterJSIModule_native_1install(
        JNIEnv *env, jobject thiz) {
    say_hello_world();
}

void setupFunctionToAccessJavaObjectForJSRuntime(
    JNIEnv *env,
    facebook::jsi::Runtime &jsRuntime
)
{
    auto get_string_from_jsi_via_jni = facebook::jsi::Function::createFromHostFunction(
            jsRuntime,
            // name of the function
            facebook::jsi::PropNameID::forAscii(jsRuntime, "get_string_from_jsi_via_jni"),
            // number of arguments that this function takes
            0,
            // need to pass env by value to lambda so it can use JAVA methods
            [env](
                    facebook::jsi::Runtime &runtime,
                    const facebook::jsi::Value &thisValue,
                    const facebook::jsi::Value *arguments,
                    size_t count)
                    -> facebook::jsi::Value
                {
                    __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "hello from get_string_from_jsi_via_jni");

                    JNIEnv *jniEnv = GetJniEnv();

                    jclass jAdapterJSIModuleClass = jniEnv->GetObjectClass(jAdapterJsiModuleObject);
                    __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "class of object toh mil gyi");

                    jmethodID jMakeNetworkCallIdMethodId = jniEnv->GetMethodID
                            (jAdapterJSIModuleClass, "get_string_from_jsi_via_jni","()Ljava/lang/String;");
                    __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "method-id tk ho gya");

                    jobject stringValueJNI = jniEnv->CallObjectMethod(jAdapterJsiModuleObject,jMakeNetworkCallIdMethodId);
                    __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "string value bhi mil gayi");

                    const char *str = jniEnv->GetStringUTFChars((jstring) stringValueJNI, NULL);
                    return Value(runtime, String::createFromUtf8(runtime, str));
                }
            );
    jsRuntime.global().setProperty(jsRuntime, "get_string_from_jsi_via_jni",
                                   std::move(get_string_from_jsi_via_jni));

    auto makeNetworkCall_get_writable_map = facebook::jsi::Function::createFromHostFunction(
            jsRuntime,
            // name of the function
            facebook::jsi::PropNameID::forAscii(jsRuntime, "makeNetworkCall_get_writable_map"),
            // number of arguments that this function takes
            0,
            // need to pass env by value to lambda so it can use JAVA methods
            [env](
                    facebook::jsi::Runtime &runtime,
                    const facebook::jsi::Value &thisValue,
                    const facebook::jsi::Value *arguments,
                    size_t count)
                    -> facebook::jsi::Value
            {
                __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "hello from makeNetworkCall_get_writable_map");

                JNIEnv *jniEnv = GetJniEnv();

                jclass jAdapterJSIModuleClass = jniEnv->GetObjectClass(jAdapterJsiModuleObject);
                __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "class of object toh mil gyi");

                jmethodID jMakeNetworkCallIdMethodId = jniEnv->GetMethodID
                        (jAdapterJSIModuleClass, "makeNetworkCall_get_writable_map","()Lcom/facebook/react/bridge/WritableMap;");
                __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "method-id tk ho gya");

                jobject networkCallResponse = jniEnv->CallObjectMethod(jAdapterJsiModuleObject,
                                                     jMakeNetworkCallIdMethodId);

                __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module",
                                    "network call response bhi mil gya");

                facebook::jsi::Value jsResponse = convertJNIObjectToJSIValue(
                            runtime,
                            adopt_local(networkCallResponse)
                        );
                __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "got jsi value from "
                                                                           "java writable map");

                return jsResponse;
            }
    );
    jsRuntime.global().setProperty(jsRuntime, "makeNetworkCall_get_writable_map",
                                   std::move(makeNetworkCall_get_writable_map));
}

void setupFunctionsForJSRuntime(
    JNIEnv *env,
    facebook::jsi::Runtime &jsRuntime
)
{
    setupFunctionToAccessJavaObjectForJSRuntime(env, jsRuntime);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_turbomoduleusage0725_rn_1vanilla_1native_1modules_AdapterJSIModule_install_1jsi_1module_1bindings(
        JNIEnv *env,
        jobject thiz,
        jlong jsi_pointer) {
    auto runtime = reinterpret_cast<facebook::jsi::Runtime *>(jsi_pointer);
    if (runtime == nullptr)
    {
        __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "Error: JS runtime is null, please check: Java_com_jsinetworklayer_JsiNetworkLayerModule impl");
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, "custom-jsi-module", "Hello from Native World! FYI Runtime Exists");

    env->GetJavaVM(&java_vm);
    jAdapterJsiModuleObject = env->NewGlobalRef(thiz);

    setupFunctionsForJSRuntime(env, *runtime);
}