#pragma once

namespace ts {

class task_17 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_pixels	= 125'000'000;
	static constexpr size_t	t_in_size	= t_pixels * 4;
	static constexpr size_t	t_out_size	= t_pixels;

public:
	task_17() : task( "Blue color from RGBA", t_divisor, t_in_size, t_out_size ) {}

	void generate_input( random_generator_t & generator ) override {
		int_distribution_t <uint32_t> dist;

		uint32_t * p_stdin = m_stdin.data<uint32_t>();
		uint8_t * p_exp = m_expected.data<uint8_t>();

		for ( size_t i = 0; i < t_pixels; i++ ) {
			uint32_t value = dist( generator );
			*p_stdin++ = value;
			*p_exp++ = value >> 16 & 0xff;
		}
	}
}; // class task_17

} // namespace ts
