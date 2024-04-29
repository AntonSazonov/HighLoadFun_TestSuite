#pragma once

namespace ts {

class task_10 : public task {
	static constexpr size_t	t_divisor	= 1'000;	// score divisor
	static constexpr size_t	t_numbers	= 1'000'000;
	static constexpr size_t	t_in_size	= t_numbers * sizeof( uint32_t );
	static constexpr size_t	t_out_size	= std::numeric_limits<uint64_t>::digits10 + 2;//1;

	constexpr uint64_t power( uint32_t a, uint32_t n, uint32_t mod ) {
		uint64_t power = a, result = 1;
		while ( n ) {
			if ( n & 1 ) result = (result * power) % mod;
			power = (power * power) % mod;
			n >>= 1;
		}
		return result;
	}

	constexpr bool witness( uint32_t a, uint32_t n ) {
		uint32_t u = n / 2;
		uint32_t t = 1;
		while ( !(u & 1) ) {
			u /= 2;
			++t;
		}

		uint64_t curr{};
		uint64_t prev = power( a, u, n );
		for ( uint32_t i = 1; i <= t; ++i ) {
			curr = (prev * prev) % n;
			if ( (curr == 1) && (prev != 1) && (prev != n - 1) ) return true;
			prev = curr;
		}

		if ( curr != 1 ) return true;
		return false;
	}

	constexpr bool is_prime( uint32_t number ) {
		if ( ((!(number & 1)) && number != 2) || (number < 2) || (number % 3 == 0 && number != 3) ) return false;

		if ( number < 1'373'653 ) {
			for ( uint32_t k = 1; 36 * k * k - 12 * k < number; ++k ) {
				if ( (number % (6*k+1) == 0) || (number % (6 * k - 1) == 0) ) return false;
			}
			return true;
		}

		if ( number < 9'080'191 ) {
			if ( witness( 31, number ) ) return false;
			if ( witness( 73, number ) ) return false;
			return true;
		}

		if ( witness(  2, number ) ) return false;
		if ( witness(  7, number ) ) return false;
		if ( witness( 61, number ) ) return false;
		return true;
	}

public:
	task_10() : task( "Sum of prime numbers", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & gen ) override {
		int_distribution_t <uint32_t> dist;

		uint32_t * p_stdin = m_stdin.data<uint32_t>();

		uint64_t sum = 0;
		for ( size_t i = 0; i < t_numbers; i++ ) {
			uint32_t value = dist( gen );
			if ( is_prime( value ) ) {
				sum += value;
			}
			*p_stdin++ = value;
		}

		int l = sprintf( m_expected.data<char>(), "%zu", sum );
		m_expected.resize( l );

		return true;
	}
}; // class task_10

} // namespace ts
