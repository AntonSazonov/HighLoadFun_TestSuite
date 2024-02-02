#pragma once

namespace ts {

// You have a data stream of numbers in string representation sending to STDIN. Each line of the flow has one number in the interval [0; 2147483647].
// Try to find the sum of these numbers for the shortest time. In case of an integer overflow, just ignore it.
class task_01 : public task {
	static constexpr size_t	t_divisor	= 10'000;	// score divisor
	static constexpr size_t	t_lines		= 50'000'000;
	static constexpr size_t	t_in_size	= t_lines * 11/*max line size (with \n)*/;
	static constexpr size_t	t_out_size	= 17;

public:
	task_01() : task( "Parse integers", t_divisor, t_in_size, t_out_size ) {}

	void generate_input( std::mt19937_64 & generator ) override {

		std::uniform_int_distribution <int32_t> dist( 0 );

		uint64_t sum = 0;

		char * p_stdin = m_stdin.data<char>();
		for ( size_t i = 0; i < t_lines; i++ ) {
			int32_t value = dist( generator );
			int len = sprintf( p_stdin, "%d\n", value );
			p_stdin += len;

			sum += value;
		}

		sprintf( m_expected.data<char>(), "%zu", sum );

#ifndef NDEBUG
		//fprintf( stderr, "sum: %zu\n", sum );
#endif

		// Test incorrect result
		//m_expected.data<char>()[0]++;
	}
}; // class task_01

} // namespace ts
