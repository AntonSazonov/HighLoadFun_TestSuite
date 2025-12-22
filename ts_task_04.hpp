#pragma once

namespace ts {

class task_04 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_numbers	= 250'000'000;
	static constexpr size_t	t_in_size	= t_numbers * sizeof( uint32_t );
	static constexpr size_t	t_out_size	= 12;

public:
	task_04() : task( "Format integers", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {
		int_distribution_t <uint32_t> dist;

		uint32_t * p_stdin = m_stdin.data<uint32_t>();
		uint64_t CRC = 0;

		size_t n = t_numbers;
		while ( n-- ) {
			uint32_t v = dist( generator );
			*p_stdin++ = v;

			// Calc. CRC...
			uint32_t l = (v >= 1000000000) ? 9 : (v >= 100000000) ? 8 : (v >= 10000000) ? 7 :
						 (v >=    1000000) ? 6 : (v >=    100000) ? 5 : (v >=    10000) ? 4 :
						 (v >=       1000) ? 3 : (v >=       100) ? 2 : (v >=       10) ? 1 : 0;

			while ( l ) {
				CRC += (v % 10 + '0') * l--;
				v /= 10;
			}
		}
		sprintf( m_expected.data<char>(), "%zu", CRC );
		return true;
	}
}; // class task_04

} // namespace ts
