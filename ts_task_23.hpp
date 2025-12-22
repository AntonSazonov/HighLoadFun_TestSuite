#pragma once

namespace ts {

class task_23 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_side_size	= 10'000;
	static constexpr size_t	t_in_size	= t_side_size * t_side_size;
	static constexpr size_t	t_out_size	= 5;		// 5 decimal digits max.

public:
	task_23() : task( "Largest Square Submatrix of all 1's", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {

		int_distribution_t <uint32_t> dist;
		uint8_t * p_stdin = m_stdin.data<uint8_t>();

		// ****************************************************************************
		// https://github.com/zara6502/stuff/blob/main/submatrixgen.cpp
		// Generate...
		//

#if 0
		std::memset( p_stdin, 0, t_in_size );
#else
		for ( uint32_t i = 0; i < t_side_size; i++ ) {
			for ( uint32_t j = 0; j < t_side_size; j++ ) {
				//square[i][j] = 0;
				p_stdin[i * t_side_size + j] = 0;
			}
		}
#endif

		const uint32_t ones_count = t_in_size * .7;		// 70% (70'000'000) of ones
		for ( uint32_t i = 0; i < ones_count; i++ ) {
			uint32_t row, col;
			do {
				row = dist( generator ) % t_side_size;
				col = dist( generator ) % t_side_size;
			} while ( p_stdin[row * t_side_size + col] );
			p_stdin[row * t_side_size + col] = 1;
		}

		// Result value
		uint32_t max_side_size = 0;

		printf( "\n" );

		const uint32_t squares_count = t_side_size * .1;	// 1000 of false squares
		const uint32_t min_size = 2;
		const uint32_t max_size = 50;
		for ( uint32_t i = 0; i < squares_count; i++ ) {

			uint32_t square_size = min_size + dist( generator ) % (max_size - min_size + 1);

			if ( square_size > max_side_size ) {
				max_side_size = square_size;
				printf( " *** square_size > max_side_size: %u\n", max_side_size );
			}

			uint32_t start_row = dist( generator ) % (t_side_size - square_size + 1);
			uint32_t start_col = dist( generator ) % (t_side_size - square_size + 1);
			for ( uint32_t r = start_row; r < start_row + square_size; r++ ) {
				for ( uint32_t c = start_col; c < start_col + square_size; c++ ) {
					p_stdin[r * t_side_size + c] = 1;
				}
			}
		}

		uint32_t big_square_size = dist( generator ) % 100 + 1;
		uint32_t big_start_row = dist( generator ) % (t_side_size - big_square_size + 1);
		uint32_t big_start_col = dist( generator ) % (t_side_size - big_square_size + 1);

		printf( " *** big_square_size: %u\n", big_square_size );
		if ( big_square_size > max_side_size ) {
			max_side_size = big_square_size;
			printf( " *** big_square_size > max_side_size: %u\n", max_side_size );
		}

		for ( uint32_t r = big_start_row; r < big_start_row + big_square_size; r++ ) {
			for ( uint32_t c = big_start_col; c < big_start_col + big_square_size; c++ ) {
				p_stdin[r * t_side_size + c] = 1;
			}
		}

#if 0
		// ****************************************************************************
		// Solve...
		{
			for ( uint32_t i = 0; i < t_side_size; i++ ) {

				printf( "i: %d\n", i ); fflush( stdout ); // !!! DEBUG !!!

				for ( uint32_t j = 0; j < t_side_size; j++ ) {

				printf( " j: %d\n", j ); fflush( stdout ); // !!! DEBUG !!!

					if ( p_stdin[i * t_side_size + j] ) {

						uint32_t current_max = 1;
						uint32_t max_possible = t_side_size - (i > j ? i : j);

						for ( uint32_t k = 1; k < max_possible; k++ ) {

							bool valid = true;
							for ( uint32_t x = 0; x <= k && valid; x++ )
							for ( uint32_t y = 0; y <= k && valid; y++ ) {
								if ( p_stdin[(i + x) * t_side_size + (j + y)] != 1 ) {
									valid = false;
								}
							}

							if ( valid ) {
								current_max = k + 1;
							} else {
								break;
							}
						}

						if ( current_max > max_size ) {
							max_size = current_max;
						}
					}
				}
			}
#if 1
#else
#endif
			m_expected.resize( sprintf( m_expected.data<char>(), "%u", max_size ) );
		}
#endif
		m_expected.resize( sprintf( m_expected.data<char>(), "%u", max_side_size ) );
		return true;
	}
}; // class task_23

} // namespace ts
