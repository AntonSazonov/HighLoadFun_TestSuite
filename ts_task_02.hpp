#pragma once

#define XXH_NO_STDLIB
#define XXH_INLINE_ALL

#if defined(__AVX512__)
 #define XXH_VECTOR XXH_AVX512
#elif defined(__AVX2__)
 #define XXH_VECTOR XXH_AVX2
#elif defined(__SSE2__)
 #define XXH_VECTOR XXH_SSE2
#else
 #define XXH_VECTOR XXH_SCALAR
#endif

#include "xxhash.h"
#include "bytell_hash_map.hpp"

struct hasher {
	uint32_t operator () ( std::string_view sv ) const {
		return XXH32( sv.data(), sv.size(), 0 );
	}
};

using set_t = ska::flat_hash_set <std::string_view, hasher>;
//using set_t = std::unordered_set <std::string>;

namespace ts {

// Unique strings
class task_02 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_lines		= 30'000'000;
	static constexpr size_t	t_line_size	= 16;		// max line size (with \n)
	static constexpr size_t	t_in_size	= t_lines * t_line_size;
	static constexpr size_t	t_out_size	= 7;

public:
	task_02() : task( "Unique strings", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {

		set_t tokens;

		std::string_view char_lut{ "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@#%*" };

		int_distribution_t <> dist_len( 1, 15 );
		int_distribution_t <> dist_chr( 0, char_lut.size() - 1 );

		char * p_stdin = m_stdin.data<char>();

		for ( size_t l = 0; l < t_lines; l++ ) {

			int line_len = dist_len( generator );

			char line[t_line_size];
			for ( int i = 0; i < line_len; i++ ) {
				line[i] = char_lut[dist_chr( generator )];
			}
			line[line_len] = '\n';

			std::memcpy( p_stdin, line, line_len + 1 );

			tokens.insert( std::string_view( p_stdin, line_len ) );

			p_stdin += line_len + 1;
		}

		strcpy( m_expected.data<char>(), std::to_string( tokens.size() ).c_str() );

		// Test incorrect result
		//m_expected.data<char>()[0]++;

		return true;
	}
}; // class task_02

} // namespace ts
