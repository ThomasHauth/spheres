#pragma once

#include <unordered_map>
#include <boost/noncopyable.hpp>
#include <SpheresEngine/DataTypes/RingBuffer.h>
#include <SpheresEngine/Log.h>
#include <chrono>

#ifdef DESCENT_PROFILE
// todo: average of the last n measurements
class SectionRepo: boost::noncopyable {
public:
	// store up to 1k performance entries
	typedef std::chrono::microseconds Duration;
	typedef RingBuffer<Duration, 10000> Durations;

	void storeResult(std::string const& name, Duration dr);

	void report();

	void clearFile( std::string const& filename);

	void dumpFile( std::string const& filename);

	void clear();

private:
	std::unordered_map<std::string, Durations> m_durations;
};

class SectionTimer: boost::noncopyable {
public:
	SectionTimer(SectionRepo & rep, std::string const& sectionName) :
	m_repo(rep), m_sectionName(sectionName) {
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	~SectionTimer() {
		auto endTime = std::chrono::high_resolution_clock::now();
		m_repo.storeResult(m_sectionName,
				std::chrono::duration_cast < std::chrono::microseconds > (endTime - m_startTime));
	}

private:
	SectionRepo & m_repo;
	std::string m_sectionName;
	std::chrono::high_resolution_clock::time_point m_startTime;

};

#else

/**
 * A time repository for a section. This is the "do nothing" implementation
 * in case profiling is not enabled in this compile
 */
class SectionRepo: boost::noncopyable {
public:
	/**
	 * do nothing
	 */
	void report() {
	}

	/**
	 * do nothing
	 */
	void clearFile(std::string const& filename) {

	}

	/**
	 * do nothing
	 */
	void clear() {

	}

	/**
	 * do nothing
	 */
	void dumpFile(std::string const& filename) {

	}
};

/**
 * A section timer. This is the "do nothing" implementation
 * in case profiling is not enabled in this compile
 */
class SectionTimer: boost::noncopyable {
public:
	/**
	 * do nothing
	 */
	SectionTimer(SectionRepo & rep, std::string const& sectionName) {
	}
};

#endif

/**
 * This class holds the global section timing information in a static member
 */
class GlobalTimingRepo {
public:
	/**
	 * Holds all section timings
	 */
	static SectionRepo Rep;
};
