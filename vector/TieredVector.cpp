#include <vector>

using namespace std;

class SolidDeque {
public:
	SolidDeque(const int size)
		: m_data(size + 1)
		, m_head(0)
		, m_tail(0)
	{}

	void push_back(const int &value) {
		m_data[m_tail] = value;
		m_tail = move(m_tail, 1);
	}

	int pop_front() {
		const int value = m_data[m_head];
		m_head = move(m_head, 1);
		return value;
	}

	void erase(const int idx) {
		int element_idx = move(m_head, idx);
		const int distance = m_tail >= element_idx ? m_tail - element_idx : (m_tail + static_cast<int>(m_data.size()));
		for (int c = 0; c < distance; c++) {
			const int next_el = move(element_idx, 1);
			m_data[element_idx] = m_data[next_el];
			element_idx = next_el;
		}
		m_tail = move(m_tail, -1);
	}

	int size() const {
		return m_tail >= m_head ? m_tail - m_head : (m_tail + static_cast<int>(m_data.size())) - m_head;
	}

	const int &operator[] (const int idx) const {
		int element_idx = move(m_head, idx);
		return m_data[element_idx];
	}

private:
	int move(const int from, const int offset) const {
		return static_cast<size_t>(from + offset) % m_data.size();
	}

private:
	vector<int> m_data;
	int m_head;
	int m_tail;
};


class TieredVector {
public:
	TieredVector(const int element_count, const int tier_factor = 10)
		: deque_size((1 << tier_factor) - 1)
		, m_data(element_count / deque_size, deque_size)
	{
		if (element_count - (deque_size * m_data.size()) > 0) {
			m_data.emplace_back(move(SolidDeque(deque_size)));
		}
		for (int c = 0; c < element_count; c++) {
			const int idx = deque_idx(c);
			m_data[idx].push_back(c);
		}
	}

	int get(const int idx) {
		const int deque_number = deque_idx(idx);
		return m_data[deque_number][idx - deque_number * deque_size];
	}

	void erase(const int idx) {
		const int deque_number = deque_idx(idx);
		const int deque_idx = idx - static_cast<int>(m_data.size()) * deque_number;
		m_data[deque_number].erase(deque_idx);
		for (int c = deque_number; c < m_data.size() - 1; c++) {
			m_data[c].push_back(m_data[c + 1].pop_front());
		}
	}

private:
	int deque_idx(const int idx) const {
		return idx / deque_size;
	}

private:
	const int deque_size;
	vector<SolidDeque> m_data;
};

typedef TieredVector Sequence;

int main() {
	return 0;
}
