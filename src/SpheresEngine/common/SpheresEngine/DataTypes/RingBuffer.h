#pragma once

#include <array>
#include <cassert>

/**
 * Ring buffer of a fixed maximal size.
 *
 * TTYpe is the data type stored in the ring buffer entries and
 * TSize the maximal size of the ringbuffer in number of entries.
 * If more entries are pushed, the first one pushed will be overwritten
 */
template<class TType, size_t TSize>
class RingBuffer {
public:

	/**
	 * push one entry to the back of the ring buffer. If the maximum number
	 * of entries is reached, the oldest entry will be overwritten
	 * @param entry the new entry to add
	 */
	void push(TType const & entry) {
		if (m_currentPosition >= TSize) {
			m_currentPosition = m_currentPosition % TSize;
		}
		m_buffer[m_currentPosition] = entry;
		m_currentPosition++;
		if (m_currentSize < m_buffer.size())
			m_currentSize++;
	}

	/** get the most recent entries with get(0)
	 * or the ones before with get ( -n )
	 * @param relativeEntryNumber the relative entry number to the
	 * item which was added most recently. 0 will return
	 * the most recent entry and -n will return the n most
	 * recent entry.
	 */
	TType const& get(int relativeEntryNumber) const {
		// subtract -1 because curren position always points to the
		// next free slot
		int absoluteItemNumber = (m_currentPosition - 1) + relativeEntryNumber;

		// handle wrap-around to the left
		while (absoluteItemNumber < 0)
			absoluteItemNumber += m_currentSize;

		// handle wrap arount to the rigtht
		if (absoluteItemNumber >= int(m_currentSize))
			absoluteItemNumber = absoluteItemNumber % m_currentSize;

		return m_buffer[absoluteItemNumber];
	}

	/**
	 * Return the last entry of the ring buffer. call this only, if you made
	 * sure there are actually entries in the buffer
	 */
	TType const& last() const {
		assert(size() > 0);
		return get(0);
	}

	/**
	 * Number of entries in the ring buffer.
	 * size can be < TSize if no items have been added, but is never bigger than
	 * TSize
	 */
	size_t size() const {
		return m_currentSize;
	}

	/**
	 * Returns true if there is at least one entry in the ring buffer
	 */
	bool hasEntries() const {
		return size() > 0;
	}

	/**
	 * Clears all entries from the buffer
	 */
	void clear() {
		m_currentSize = 0;
		m_currentPosition = 0;
	}

private:

	/**
	 * Checks if the current position is larger than the total size and
	 * makes sure the current position is wrapped to the actual range
	 */
	void handleCurrentPositionWrapAround() {
	}

	std::array<TType, TSize> m_buffer;
	size_t m_currentPosition = 0;
	size_t m_currentSize = 0;
};

