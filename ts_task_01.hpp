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
		int_distribution_t <int32_t> dist;
//		int_distribution_t <int32_t> dist( 1, std::numeric_limits<int32_t>::max() );

		uint64_t sum = 0;

		char * p_stdin = m_stdin.data<char>();

#if 0
		int32_t vmin = 1'000'000;
		int32_t vmax = 0;
		int32_t vzero = 0;
#endif

		for ( size_t i = 0; i < t_lines; i++ ) {
			int32_t value = dist( generator );
			sum += value;

#if 0
			if ( !value ) vzero++;
			if ( value > vmax ) vmax = value;
			if ( value < vmin ) vmin = value;
#endif

			// Faster than sprintf.
			std::string s = std::to_string( value );

			if ( value > 2147483647 || s.size() > 10 ) {
				fprintf( stderr, " !!!!!!!!!!!!!!!\n" );
			}

			for ( char ch : s ) *p_stdin++ = ch;
			*p_stdin++ = '\n';
		}

		m_stdin.resize( p_stdin - m_stdin.data<char>() );

		sprintf( m_expected.data<char>(), "%zu", sum );

//		fprintf( stderr, "xmin/max: [%d-%d], %d\n", vmin, vmax, vzero );

#if 0
		char * p = m_stdin.data<char>();
		size_t n = t_lines;
		size_t i = 0;
		for ( ; n; ) {

			char c = *p++;
			if ( c != '\n' && c < '0' && c > '9' ) {
				fprintf( stderr, " !!!!!!!!!!!!!!! c: '%c', %d, (0x%x)\n", c, int(c), int(c) );
				break;
			}

			if ( c == '\n' ) {
				n--;
				i++;
			}
		}
		fprintf( stderr, "\ni: %zu\n", i );
		fprintf( stderr, "sum: %zu\n", sum );
#endif

#ifndef NDEBUG
		//fprintf( stderr, "sum: %zu\n", sum );
#endif

		// Test incorrect result
		//m_expected.data<char>()[0]++;
	}
}; // class task_01

} // namespace ts
