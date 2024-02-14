#pragma once

namespace ts {

// Unique strings
class task_02 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_lines		= 30'000'000;
	static constexpr size_t	t_line_size	= 16; // max line size (with \n)
	static constexpr size_t	t_in_size	= t_lines * t_line_size;
	static constexpr size_t	t_out_size	= 7;

public:
	task_02() : task( "Unique strings", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {

	    std::unordered_set <std::string> tokens;

		constexpr const char * chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@#%*";

		int_distribution_t <> dist_len( 1, 15 );
		int_distribution_t <> dist_chr( 0, strlen( chars ) - 1 );

		char * p_stdin = m_stdin.data<char>();

		for ( size_t l = 0; l < t_lines; l++ ) {

			int line_len = dist_len( generator );

			char line[t_line_size];
			for ( int i = 0; i < line_len; i++ ) {
				line[i] = chars[dist_chr( generator )];
			}
			line[line_len] = '\n';

			tokens.insert( std::string( line, line_len ) );

			std::memcpy( p_stdin, line, line_len + 1 );
			p_stdin += line_len + 1;
		}

//		std::string expected = std::to_string( tokens.size() );
//		strcpy( m_expected.data<char>(), expected.c_str() );

		strcpy( m_expected.data<char>(), std::to_string( tokens.size() ).c_str() );

		// Test incorrect result
		//m_expected.data<char>()[0]++;

		return true;
	}
}; // class task_02

} // namespace ts
