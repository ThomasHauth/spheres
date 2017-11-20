#pragma once

#include <utility>

class Resolution {
public:
	Resolution(int w, int h) :
			m_res(w, h) {

	}

	int w() const {
		return m_res.first;
	}

	int h() const {
		return m_res.second;
	}

	float ratio() const {
		return float(w()) / float(h());
	}

private:
	std::pair<int, int> m_res;
};
