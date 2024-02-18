#pragma once

namespace ts {

class task_19 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_pixels	= 150'000'000;
	static constexpr size_t	t_in_size	= t_pixels * 3;
	static constexpr size_t	t_out_size	= t_pixels;

public:
	task_19() : task( "Blue color from RGB", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {
		int_distribution_t <uint32_t> dist;

		uint8_t *	p_stdin	= m_stdin.data<uint8_t>();
		uint8_t *	p_exp	= m_expected.data<uint8_t>();

		for ( size_t i = 0; i < t_pixels; i++ ) {
			uint32_t c = dist( generator );
			uint8_t r = c       & 0xff;
			uint8_t g = c >>  8 & 0xff;
			uint8_t b = c >> 16 & 0xff;
			*p_stdin++ = r;
			*p_stdin++ = g;
			*p_stdin++ = b;
			*p_exp++ = b;
		}

		return true;
	}
}; // class task_19

} // namespace ts
