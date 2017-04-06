#include <jni.h>
#include <string>

#include <SpheresEngine/AndroidVRFramework.h>

#include "VRTestBenchFramework.h"

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_onetwofivegames_spherestestbenchandroidvr_SpheresVRActivity_##method_name

namespace {

    inline jlong jptr(AndroidVRFramework *native_framework) {
        return reinterpret_cast<intptr_t>(native_framework);
    }

    inline AndroidVRFramework *native(jlong ptr) {
        return reinterpret_cast<AndroidVRFramework *>(ptr);
    }
}  // anonymous namespace

extern "C" {

JNI_METHOD(jlong, nativeCreateFramework)(JNIEnv *env, jclass clazz,
                                        jobject class_loader,
                                        jobject android_context,
                                        jlong native_gvr_api) {
    std::unique_ptr<gvr::AudioApi> audio_context(new gvr::AudioApi);
    audio_context->Init(env, android_context, class_loader,
                        GVR_AUDIO_RENDERING_BINAURAL_HIGH_QUALITY);

    return jptr(
             new VRTestBenchFramework (reinterpret_cast<gvr_context *>(native_gvr_api),
                                      std::move(audio_context))
            /* new AndroidVRFramework(reinterpret_cast<gvr_context *>(native_gvr_api),
                                      std::move(audio_context))
                                      */
                                     );
}

JNI_METHOD(void, nativeDestroyRenderer)
(JNIEnv *env, jclass clazz, jlong native_framework) {
    delete native(native_framework);
}

JNI_METHOD(void, nativeInitializeGl)(JNIEnv *env, jobject obj,
                                     jlong native_framework) {
    //native(native_framework)->InitializeGl();
    // creates the RenderBackendAndroidVR but does not actually create the RenderEngine
    // and all its friends like the resource engine. This is done later
    // This is done here, because Android tells us to setup the surface here
    native(native_framework)->setJavaInterface( env, obj );
    native(native_framework)->createVRRenderBackend();
}

JNI_METHOD(void, nativeDrawFrame)(JNIEnv *env, jobject obj,
                                  jlong native_framework) {
    // initialized game and draws if not done yet.
    native(native_framework)->setJavaInterface( env, obj );
    native(native_framework)->drawFrame();
}

JNI_METHOD(void, nativeOnTriggerEvent)(JNIEnv *env, jobject obj,
                                       jlong native_framework) {
    //native(native_framework)->OnTriggerEvent();
    native(native_framework)->setJavaInterface( env, obj );
}

JNI_METHOD(void, nativeOnPause)(JNIEnv *env, jobject obj,
                                jlong native_framework) {
    native(native_framework)->setJavaInterface( env, obj );
    native(native_framework)->pauseGame();
}

JNI_METHOD(void, nativeOnResume)(JNIEnv *env, jobject obj,
                                 jlong native_framework) {
    native(native_framework)->setJavaInterface( env, obj );
    native(native_framework)->resumeGame();
}

}  // extern "C"
