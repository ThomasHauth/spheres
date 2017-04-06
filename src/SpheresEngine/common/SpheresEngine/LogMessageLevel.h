#pragma once

#include <SpheresEngine/LogInternal.h>
#include <SpheresEngine/LogMessageLevel.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <cassert>

namespace logging {

enum class MessageLevel {
	Debug, Normal, Error, Fatal
};

}
