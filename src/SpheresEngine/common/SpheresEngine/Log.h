#pragma once

#include <SpheresEngine/LogInternal.h>
#include <SpheresEngine/LogMessageLevel.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <sstream>
#include <cassert>

namespace logging {

namespace Categories {

/**
 * Logging category of the Game loop
 */
class GameLoop {
public:
	/**
	 * enable this category via an integer
	 */
	static constexpr size_t isEnabled = 1;
};

/**
 * Logging category of the Render engine
 */
class RenderEngine {
public:
	/**
	 * enable this category via an integer
	 */
	static constexpr size_t isEnabled = 1;
};

/**
 * Default logging category
 */
class Default {
public:
	/**
	 * enable this category via an integer
	 */
	static constexpr size_t isEnabled = 1;
};
}

/**
 * Base implementation for all of the loggin subsystem
 */
template<MessageLevel TLevel, class TLogCategory = Categories::Default>
class LogImplBase {
public:
	/**
	 * Request explicit ctor usage to prevent accidental initializations
	 */
	explicit LogImplBase() {
	}

	/**
	 * dtor actually outputs the compiled text stream to the logging output
	 * device
	 */
	~LogImplBase() {
		//if (TLogCategory::isEnabled)
		logging::_internal_log(m_stream.str(), TLevel);
		assert(TLevel != MessageLevel::Fatal);
	}

	/**
	 * pushes new logging content onto the text stream and returns
	 * a reference to this class to enable further usage of <<
	 */
	template<class T>
	LogImplBase &
	operator<<(T const & t) {
		if (TLogCategory::isEnabled)
			m_stream << t;
		return *this;
	}

protected:
	/**
	 * This stringstream collects all logging content that has been pushed
	 * on the loging object
	 */
	std::stringstream m_stream;

	/**
	 * Store the message level
	 */
	MessageLevel m_msgLevel = MessageLevel::Normal;
};

/**
 * performs no logging
 * gets optimized out by compiler
 */
template<MessageLevel TLevel, class TLogCategory = Categories::Default>
class LogNullBase {
public:
	explicit LogNullBase() {
	}

	/**
	 * Don't output anything
	 */
	template<class T>
	LogNullBase &
	operator<<(T const &) {
		return *this;
	}
};

#ifdef SPHERES_LOG_ENABLED

using Info = LogImplBase< MessageLevel::Normal, Categories::Default>;
template<typename TCat>
using InfoC = LogImplBase< MessageLevel::Normal, TCat >;
/*
 using Debug = LogImplBase< MessageLevel::Debug, Categories::Default>;
 template<typename TCat>
 using DebugC = LogImplBase< MessageLevel::Debug, TCat >;
 */
using Debug = LogNullBase< MessageLevel::Debug, Categories::Default>;
template<typename TCat>
using DebugC = LogNullBase< MessageLevel::Debug, TCat >;

using Error = LogImplBase<MessageLevel::Error,Categories::Default>;
using Fatal = LogImplBase<MessageLevel::Fatal,Categories::Default>;

class NotImplemented: Fatal {
public:
	explicit NotImplemented() {
		this->m_stream << "Method not implemented";
	}
};

#else

/** disable all logging, these templates will be completely removed by
 * the optimizer
 * the fatal loggings will still be regular
 */
using Info = LogNullBase< MessageLevel::Normal, Categories::Default>;
template<typename TCat>
using InfoC = LogNullBase< MessageLevel::Normal, TCat >;

using Debug = LogNullBase< MessageLevel::Debug, Categories::Default>;
template<typename TCat>
using DebugC = LogNullBase< MessageLevel::Debug, TCat >;

using Error = LogNullBase<MessageLevel::Error,Categories::Default>;
using Fatal = LogImplBase<MessageLevel::Fatal,Categories::Default>;

using NotImplemented = Fatal;

#endif

}
