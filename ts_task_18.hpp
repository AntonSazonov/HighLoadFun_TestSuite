#pragma once

namespace ts {

class task_18 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_N			= 2'000;
	static constexpr size_t	t_N2		= t_N * t_N;
	static constexpr size_t	t_in_size	= t_N2 * 4/*sizeof(uint32_t)*/ * 2/*N matrices*/;
	static constexpr size_t	t_out_size	= t_in_size / 2;

public:
	task_18() : task( "Large matrix multiplication", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {
		int_distribution_t <uint32_t> dist;

		uint32_t * pA = m_stdin.data<uint32_t>();
		uint32_t * pB = pA + t_N2;
		uint32_t * pR = m_expected.data<uint32_t>();

#if 0
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += matrix_a[i][k] * matrix_b[k][j];
            }
            matrix_c[i][j] = sum;
        }
    }
#endif
		uint32_t * pT = new (std::nothrow) uint32_t [t_N2];
		if ( !pT ) return false;

		// Fill randoms...
		for ( size_t i = 0; i < t_N2; i++ ) {
			pA[i] = dist( generator );
			pT[i] = dist( generator );
		}

		// Multiply...
		for ( size_t i = 0; i < t_N; ++i )
		for ( size_t j = 0; j < t_N; ++j ) {
			int sum = 0;
			for ( size_t k = 0; k < t_N; ++k ) {
				sum += pA[i * t_N + k] * pT[j * t_N + k];
			}
			pR[i * t_N + j] = sum;
		}

		// Transpose T to B...
		for ( size_t i = 0; i < t_N; ++i )
		for ( size_t j = 0; j < t_N; ++j ) {
			pB[j * t_N + i] = pT[i * t_N + j];
		}

		delete [] pT;

#if 0
		for ( size_t i = 0; i < t_N2 * 2; i++ ) {
			pA[i] = dist( generator );
		}

		printf( "*" );
		fflush( stdout );

		for ( size_t i = 0; i < t_N; i++ ) {
			for ( size_t j = 0; j < t_N; j++ ) {
				int sum = 0;
				for ( size_t k = 0; k < t_N; k++ ) {
					sum += pA[i * t_N + k] * pB[k * t_N + j];
				}
				pR[i * t_N + j] = sum;
			}
		}
#endif
		return true;
	}
}; // class task_18

} // namespace ts
