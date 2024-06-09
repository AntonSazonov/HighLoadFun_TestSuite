#pragma once

namespace ts {

class task_05 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_numbers	= 250'000'000;
	static constexpr size_t	t_in_size	= t_numbers;
	static constexpr size_t	t_out_size	= 10;

public:
	task_05() : task( "Count uint8", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {
		int_distribution_t <uint64_t> dist;

		uint32_t num_127s = 0;

		uint64_t * p_stdin = m_stdin.data<uint64_t>();

		for ( size_t i = 0; i < t_numbers / 8; i++ ) {
			uint64_t value = dist( generator );
			*p_stdin++ = value;

			while ( value ) {
				if ( (value & 0xff) == 127 ) num_127s++;
				value >>= 8;
			}
		}

		sprintf( m_expected.data<char>(), "%u", num_127s );
		return true;
	}
}; // class task_05

} // namespace ts
