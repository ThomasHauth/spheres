#pragma once

#include <cassert>
#include <algorithm>
#include <vector>
#include <memory>
#include <functional>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_categories.hpp>

/**
 * Wrapped to easier use the storage pair of the StaticVector
 */
template<class TData>
using FilledEntryPair = std::pair<bool, TData>;

/**
 * Itetrator class for the StaticVector
 */
template<class TData, class TVectorClassRef>
class StaticVectorIteratorBase: public boost::iterator_facade<
		StaticVectorIteratorBase<TData, TVectorClassRef>, size_t
		// right now, only implement forward traversal, but in general
		// more modes are possible
		, boost::forward_traversal_tag, TData> {
public:
	/**
	 * Create the iterator and store th eref to the actual StaticVector being
	 * iterated on the the current position
	 */
	explicit StaticVectorIteratorBase(TVectorClassRef vec, size_t curPos) :
			m_vec(vec), m_curPos(curPos) {
	}

	/**
	 * Return the current position of this iterator
	 */
	size_t currentPosition() const {
		return m_curPos;
	}

private:
	friend class boost::iterator_core_access;

	/**
	 * Increase current Pos as long as we find a new filled entry
	 * or the end of the vector
	 */
	void increment() {
		m_curPos++;
		while ((m_vec.size() < m_curPos) && !m_vec[m_curPos].first) {
			m_curPos++;
		}
	}

	/**
	 * Check for equality of two iterators (having the same StaticVector backend
	 * and the same position
	 */
	bool equal(
			StaticVectorIteratorBase<TData, TVectorClassRef> const& other) const {
		return (this->m_vec == other.m_vec)
				&& (this->m_curPos == other.m_curPos);
	}

	/**
	 * Return the reference to the StaticVector which is iterated on
	 */
	TData & dereference() const {
		return m_vec[m_curPos].second;
	}

	/**
	 * Holds reference to the StaticVector iterated on
	 */
	TVectorClassRef m_vec;

	/**
	 * The current position on the static vector
	 */
	size_t m_curPos;
};

template<class TData>
using StaticVectorIterator = StaticVectorIteratorBase<TData, std::vector<FilledEntryPair<TData>> &>;

template<class TData>
using StaticVectorConstIterator = StaticVectorIteratorBase<TData, std::vector<FilledEntryPair<TData>> const&>;

/**
 * A special container which contains an orderer amount of items and is in a continous
 * memory region (like a vector), but is not down-sized and is not copied if an item is removed
 * As the downsize, new elements might not be placed at the end of the memory region but somewhere in the middle
 */
template<class TData>
class StaticVector {
public:

	/**
	 * Push one new item on the vector.
	 */
	void push(TData const& newEntry) {
		// find a free spot in the current vector, if possible
		auto freeSpot = std::find_if(m_entries.begin(), m_entries.end(),
				[]( FilledEntryPair<TData> const& e ) {return !e.first;});

		// no free space found, so grow the vector
		if (freeSpot == m_entries.end()) {
			m_entries.emplace_back(std::make_pair(true, newEntry));
		} else {
			freeSpot->first = true;
			freeSpot->second = newEntry;
		}
	}

	/**
	 * Erase one item from the vector
	 */
	void erase(StaticVectorIterator<TData> it) {
		// just mark this item as erase, but leave it in the list for now
		m_entries[it.currentPosition()].first = false;
	}

	/**
	 * Return the current size of the vector
	 */
	size_t size() const {
		// just count the filled entries
		return std::count_if(m_entries.begin(), m_entries.end(),
				[](FilledEntryPair<TData> const& d) {return d.first;});
	}

	/**
	 * Get one entry by index. This is absolute index of an entry which must
	 * consider that there might be slots on this vector which are not filled
	 */
	TData & get(size_t index) {
		return m_entries[index];
	}

	/**
	 * Return non-const iterator to the end of the vector
	 */
	StaticVectorIterator<TData> begin() {
		return StaticVectorIterator<TData>(m_entries, 0);
	}

	/**
	 * Return const iterator to the beginning of the vector
	 */
	StaticVectorConstIterator<TData> begin() const {
		return StaticVectorConstIterator<TData>(m_entries, 0);
	}

	/**
	 * Return non-const iterator to the end of the vector
	 */
	StaticVectorIterator<TData> end() {
		// an iterator with an index larger than then the last element is the end
		return StaticVectorIterator<TData>(m_entries, m_entries.size());
	}

	/**
	 * Return const iterator to the end of the vector
	 */
	StaticVectorConstIterator<TData> end() const {
		// an iterator with an index larger than then the last element is the end
		return StaticVectorConstIterator<TData>(m_entries, m_entries.size());
	}

private:

	/**
	 * Vector storing the actual entries
	 */
	std::vector<FilledEntryPair<TData>> m_entries;
};
