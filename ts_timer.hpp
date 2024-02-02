#pragma once

namespace ts {

// Timer that counts 'user + system' time of processes in microseconds

// RUSAGE_SELF
// RUSAGE_CHILDREN
// RUSAGE_THREAD
template <int Who = RUSAGE_CHILDREN>
class timer {
	uint64_t	m_time_start;

public:
	timer() : m_time_start( get() ) {}

	void reset() {
		m_time_start = get();
	}

	// Microseconds
	uint64_t us() const {
		return get() - m_time_start;
	}

	// Nanoseconds (for divisor)
	uint64_t ns() const {
		return us() * 1000;
	}

	static uint64_t get() {
		rusage res;
		getrusage( Who, &res );
		uint64_t usr = res.ru_utime.tv_sec * 1'000'000 + res.ru_utime.tv_usec;
		uint64_t sys = res.ru_stime.tv_sec * 1'000'000 + res.ru_stime.tv_usec;
		return usr + sys;
	}
}; // class timer

} // namespace ts
