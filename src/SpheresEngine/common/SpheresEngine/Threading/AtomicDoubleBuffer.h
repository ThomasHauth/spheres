#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <array>

/**
 * Emulates an atomic object in read-mode by using
 * a double buffer and an atomic pointer. One thread
 * can only read the front object which another is writing
 * to the back object. The swap() can be performed when both
 * threads are currently not using the objects.
 *
 * Todo: this can also be implemeneted via a lock-free queue,
 * where the writing thread pushes the most recent object and the
 * reading thread gets on owning pointer and the object is destroyed
 * as soon as the pointer goes out of scope.
 */
template<class TPayload>
class AtomicDoubleBuffer {

	/*
	 *  this can be called by the reading thread and returns
	 *  a pointer to read form the object which is in read mode
	 */
	TPayload const* getFront() {
		std::lock_guard<std::mutex> thisLock(m_payloadAccess);
		return &m_payload[m_frontIndex];
	}

	/**
	 * This can be called by the writing thread to get access
	 * to the object which can be safely written to
	 */
	TPayload * getBack() {
		std::lock_guard<std::mutex> thisLock(m_payloadAccess);
		return &m_payload[m_frontIndex];
	}

	/**
	 *	Return the index to the object which is currently at the back
	 */
	size_t getBackIndex() const {
		return m_frontIndex == 0 ? 1 : 0;
	}

	/**
	 * Swap the back and the front object. This method should only
	 * be called when none of the threads reads or writes
	 */
	void swap() {
		std::lock_guard<std::mutex> thisLock(m_payloadAccess);

		m_frontIndex++;
		if (m_frontIndex > 1)
			m_frontIndex = 0;
	}

private:

	/**
	 * This array contains the two payloads for the double buffer
	 */
	std::array<TPayload, 2> m_payload;

	/**
	 * Index to the object which is currently at the front
	 */
	size_t m_frontIndex;

	/**
	 * Mutex to ensure that the index is not changed while the
	 * pointer is retrieved
	 */
	std::mutex m_payloadAccess;
};
