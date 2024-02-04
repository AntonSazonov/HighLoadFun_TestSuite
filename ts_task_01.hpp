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

	void generate_input( random_generator_t & generator ) override {
		int_distribution_t <int32_t> dist( 0 );

		uint64_t sum = 0;

		char * p_stdin = m_stdin.data<char>();

		for ( size_t i = 0; i < t_lines; i++ ) {
			int32_t value = dist( generator );
			sum += value;

			// Faster than sprintf.
			std::string s = std::to_string( value );
			for ( char ch : s ) *p_stdin++ = ch;
			*p_stdin++ = '\n';
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
