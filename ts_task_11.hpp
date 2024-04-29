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
		auto gen = [&]{ return dist( generator ); };
		std::generate( p, p + t_numbers, gen );

//		m_expected.resize( sprintf( m_expected.data<char>(), "%zu", sum ) );

		return true;
	}
}; // class task_11

} // namespace ts
