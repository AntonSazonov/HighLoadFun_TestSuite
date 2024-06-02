#pragma once

namespace ts {

class task_09 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_UUIDs		= 18'000'000;
	static constexpr size_t	t_UUID_len	= 36;
	static constexpr size_t	t_line_len	= t_UUID_len + 1/*'\n'*/;
	static constexpr size_t	t_size		= t_UUIDs * t_line_len;

public:
	task_09() : task( "Sort UUIDs", t_divisor, t_size, t_size ) {}

	bool generate_input( random_generator_t & gen ) override {
		int_distribution_t <uint64_t> dist;

		char * p_src = m_stdin.data<char>();
		size_t n = t_UUIDs;
		while ( n-- ) {
			// ~13 secs.
			uint64_t x = dist( gen );
			uint64_t y = dist( gen );

			uint32_t a = x >> 32;
			uint16_t b = x >> 16 & 0xffff;
			uint16_t c = x       & 0xffff;

			uint16_t d = y >> 16 & 0xffff;
			uint16_t e = y       & 0xffff;
			uint32_t f = y >> 32;

			p_src += sprintf( p_src, "%08x-%04x-%04x-%04x-%04x%08x\n", a, b, c, d, e, f );

			//fmt::format_to( p_src, "{:08x}-{:04x}-{:04x}-{:04x}-{:04x}{:08x}\n", a, b, c, d, e, f );
			//p_src += t_line_len;

			// "8ff38a09-f995-467b-9d07-5536f621402e\n";
			//fmt::format_to( p_src, "{:08x}-{:04x}-{:04x}-{:04x}-{:04x}{:04x}{:04x}\n", a( gen ), b( gen ), c( gen ), d( gen ), e( gen ), f( gen ), g( gen ) );
			//p_src += t_line_len;

			//p_src += sprintf( p_src, "%08x-%04x-%04x-%04x-%04x%04x%04x\n", a( gen ), b( gen ), c( gen ), d( gen ), e( gen ), f( gen ), g( gen ) );
		}
		printf( "G" ); // Generated
		fflush( stdout );

//		std::memcpy( m_expected.data<char>(), m_stdin.data<char>(), t_size );

		uint32_t * p_offsets = new (std::nothrow) uint32_t [t_UUIDs];
		if ( !p_offsets ) {
			fprintf( stderr, "p_offsets allocation failed.\n" );
			return false;
		}

		// Initialize indices...
		for ( size_t i = 0; i < t_UUIDs; i++ ) {
			p_offsets[i] = i * t_line_len;
		}

		// Sort indices...
#if 1
		std::sort( p_offsets, p_offsets + t_UUIDs, [&]( uint32_t a, uint32_t b ) -> bool {
				return std::string_view( m_stdin.data<char>() + a, t_UUID_len )
					 < std::string_view( m_stdin.data<char>() + b, t_UUID_len );
			} );
#else
		std::sort( p_offsets, p_offsets + t_UUIDs, [&]( uint32_t a, uint32_t b ) -> bool {
				constexpr const uint8_t shuffle_mask[16] = { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
				const __m128i mask = *((const __m128i *)shuffle_mask);

				const __m128i va = _mm_shuffle_epi8( _mm_loadu_si128( (const __m128i *)(m_stdin.data<char>() + a) ), mask );
				const __m128i vb = _mm_shuffle_epi8( _mm_loadu_si128( (const __m128i *)(m_stdin.data<char>() + b) ), mask );

				int l = _mm_movemask_epi8( _mm_cmplt_epi8( va, vb ) );
				int g = _mm_movemask_epi8( _mm_cmpgt_epi8( va, vb ) );

				return l > g;
			} );
#endif
		printf( "S" ); // Sorted
		fflush( stdout );

		// Check result...
		bool ret = std::is_sorted( p_offsets, p_offsets + t_UUIDs, [&]( uint32_t a, uint32_t b ) {
				return std::string_view( m_stdin.data<char>() + a, t_UUID_len )
					 < std::string_view( m_stdin.data<char>() + b, t_UUID_len );
			} );
		if ( !ret ) {
			fprintf( stderr, "std::is_sorted(): false\n" );
		}
		printf( "C" ); // Checked
		fflush( stdout );

		// Copy UUIDs to new positions...
		for ( size_t i = 0; i < t_UUIDs; i++ ) {
			std::memcpy( (void *)(m_expected.data<char>() + i * t_line_len),
									 m_stdin.data<char>() + p_offsets[i], t_line_len );
		}

#if 0
		// Print first 10 lines...
		printf( "\n *** First 10 UUIDs:\n" );
		for ( int i = 0; i < 10; i++ ) {
			//fprintf( stdout, "%.*s", 37, sv.data() );
			printf( "%.*s", 37, m_expected.data<char>() + i * t_line_len );
		}
		printf( " ...\n" );
		fflush( stdout );
#endif

		delete [] p_offsets;
		return ret;
	}
}; // class task_09

} // namespace ts
