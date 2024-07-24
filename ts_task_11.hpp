#pragma once

namespace ts {

class task_11 : public task {
	static constexpr size_t	t_divisor	= 1'000;
	static constexpr size_t	t_numbers	= 100'000'000;
	static constexpr size_t	t_in_size	= t_numbers * sizeof( uint32_t );
	static constexpr size_t	t_out_size	= std::numeric_limits<uint64_t>::digits10 + 2;//1;

public:
	task_11() : task( "TopK", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {

		int_distribution_t <uint32_t> dist;
		uint32_t * p = m_stdin.data<uint32_t>();


		uint32_t lower = 4'000'000'000;
		uint32_t last_min = lower;

		std::multiset <uint32_t> s;
		for ( int i = 0; i < 100; i++ ) s.insert( lower );

		auto check = [&]( uint32_t value ) {
				if ( value > last_min ) {
					s.erase( s.begin() );
					s.insert( value );
					last_min = *s.begin();
				}
			};


//		auto gen = [&]{ return dist( generator ); };
//		std::generate( p, p + t_numbers, gen );

		for ( size_t i = 0; i < t_numbers; i++ ) {
			uint32_t v = dist( generator );
			*p++ = v;
			check( v );
		}

		uint64_t sum = 0;
		for ( uint32_t v : s ) sum += v;

		m_expected.resize( sprintf( m_expected.data<char>(), "%zu", sum ) );

//		int l = sprintf( m_expected.data<char>(), "%zu", sum );
//		m_expected.resize( l );

		return true;
	}
}; // class task_11

} // namespace ts
