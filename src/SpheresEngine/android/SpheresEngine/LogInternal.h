#pragma once

#include <SpheresEngine/LogMessageLevel.h>

#include <android/log.h>
#define ANDROID_LOGI( cstr ) ((void)__android_log_print(ANDROID_LOG_INFO, "spheres", "%s", cstr ))
#define ANDROID_LOGW( cstr ) ((void)__android_log_print(ANDROID_LOG_WARN, "spheres", "%s", cstr ))
#define ANDROID_LOGE( cstr ) ((void)__android_log_print(ANDROID_LOG_ERROR, "spheres", "%s", cstr ))


#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <cassert>

namespace logging {
inline void _internal_log(std::string message, MessageLevel lvl) {
	if ( ( lvl == MessageLevel::Fatal ) || (lvl == MessageLevel::Error) ) {
		ANDROID_LOGE ( message.c_str());
	}
	else {
		ANDROID_LOGI ( message.c_str());
	}
}
}
