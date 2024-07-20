#pragma once

namespace ts {

class task_08 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_numbers	= 100'000'000;
	static constexpr size_t	t_in_size	= t_numbers * sizeof( uint32_t );
	static constexpr size_t	t_out_size	= 10;

public:
	task_08() : task( "Median", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {

		uint32_t * p_sorted = new (std::nothrow) uint32_t [t_numbers];
		if ( !p_sorted ) {
			fmt::print( stderr, "Memory allocation error.\n" );
			return false;
		}

		// Generate...
		int_distribution_t <uint32_t> dist;
		uint32_t * p_stdin = m_stdin.data<uint32_t>();
		for ( size_t i = 0; i < t_numbers; i++ ) {
			*p_stdin++ = dist( generator );
		}


		std::memcpy( (void *)p_sorted, m_stdin.data<void *>(), t_in_size );

		std::sort( p_sorted, p_sorted + t_numbers );//
#if 0
, [&]( uint32_t a, uint32_t b ) -> bool {
				return std::string_view( m_stdin.data<char>() + a, t_UUID_len )
					 < std::string_view( m_stdin.data<char>() + b, t_UUID_len );
			} );
#endif

		sprintf( m_expected.data<char>(), "%u", p_sorted[50'000'000]);
		return true;
	}
}; // class task_08

} // namespace ts
