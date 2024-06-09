#pragma once

namespace ts {

// std::mt19937_64
// __gnu_cxx::sfmt19937_64 (from #include <ext/random>)
using random_generator_t	= __gnu_cxx::sfmt19937_64;

template <typename T = int32_t>
using int_distribution_t	= std::uniform_int_distribution <T>;


// TODO: add shm for stderr?
class task {
	std::string	m_name;
	int			m_score_divisor;

protected:
	ts::shm		m_stdin;
	ts::shm		m_stdout;
	ts::shm		m_expected;

public:
	task( const std::string & name, int score_divisor, size_t stdin_max_size, size_t stdout_max_size )
		: m_name( name )
		, m_score_divisor( score_divisor )
		, m_stdin   ( "/ts_stdin"   , stdin_max_size )
		, m_stdout  ( "/ts_stdout"  , stdout_max_size )
		, m_expected( "/ts_expected", stdout_max_size ) {}

	virtual ~task() = default;

	constexpr explicit operator bool () const {
		return m_stdin && m_stdout && m_expected;
	}

	//std::string name()	const { return m_name; }
	//int score_divisor()	const { return m_score_divisor; }

	virtual bool generate_input( random_generator_t & ) = 0;

	virtual bool check_result() {

		// chech only N first chars (m_expected.size())
		return m_stdout.mem_view<char>() == m_expected.mem_view<char>();
	}

	bool run( const std::string & executable, int iterations = 3 ) {

		printf( " # Testing '%s', %d iteration%s...\n", m_name.c_str(), iterations, iterations > 1 ? "s" : "" );

		std::random_device	rd;
		std::seed_seq		seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
		random_generator_t	random_generator( seed );

		ts::timer	timer_child;

		uint64_t	time_sum = 0;
		uint64_t	score_sum = 0;
		uint64_t	time_best = -1;
		uint64_t	score_best = 0;

		for ( int i = 0; i < iterations; i++ ) {

			// Truncate to reserved size
			m_stdin.reset();

			m_stdin.rewind();
			m_stdout.rewind();

			printf( "\n Iteration #%2d:\n", i + 1 );
			printf( "   Generating input data... " );
			fflush( stdout );

			// Child process timer...
			ts::timer <RUSAGE_SELF> timer_self;

			if ( !generate_input( random_generator ) ) {
				fprintf( stderr, "\ngenerate_input(): failed\n" );
				return false;
			}

			printf( " %.2f secs., size: %zu bytes\n", timer_self.us() / 1e6, m_stdin.size() );
			//fflush( stdout );

			{
				ts::scoped_fd_subst dup_stdin ( STDIN_FILENO , m_stdin .fd() );
				ts::scoped_fd_subst dup_stdout( STDOUT_FILENO, m_stdout.fd() );

				timer_child.reset();
				//if ( system( ("./" + executable + " 2> " + executable + "_stderr.log").c_str() ) == -1 ) {
				if ( system( ("./" + executable).c_str() ) == -1 ) {
					perror( "system()" );
					return false;
				}

				fflush( stdout );
			}

#if 0
			fprintf( stderr, "   m_stdin.size(): %zu\n",    m_stdin.size() );
			fprintf( stderr, "  m_stdout.size(): %zu\n",   m_stdout.size() );
			fprintf( stderr, "m_expected.size(): %zu\n", m_expected.size() );
#endif

			uint64_t	time_ns		= timer_child.ns();
			double		time_sec	= time_ns / 1e9;
			uint64_t	score		= time_ns / m_score_divisor;
			time_sum += time_ns;
			score_sum += score;
			if ( time_ns < time_best ) {
				time_best  = time_ns;
				score_best = score;
			}

			bool is_correct = check_result();

			if ( m_expected.mem_view<char>().size() < 80 ) {
				printf( "       Expected: [%.*s]\n", int(m_expected.mem_view<char>().size()), m_expected.mem_view<char>().data() );
				printf( "            Got: [%.*s]\n", int(m_stdout  .mem_view<char>().size()), m_stdout  .mem_view<char>().data() );
			}

			fmt::print( "         Result: " );
			if ( is_correct ) {
				fmt::print( fg(fmt::color::light_green), "passed\n" );
			} else {
				fmt::print( fg(fmt::color::red), "not passed\n" );
			}
			printf( "          Score: %zu\n", score );
			printf( "           Time: %5.2f secs.\n", time_sec/*time_ns / 1e9*/ );
			printf( "     stdin size: %zu bytes\n", m_stdin.size() );
			printf( "     stdin read: %5.2f GB/s\n", m_stdin.size() / time_sec / (1024 * 1024 * 1024) );

//			if ( !is_correct ) return false;
		}
		printf( "\n  Avarage time : %5.2f secs.\n", time_sum / 1e9 / iterations );
		printf( "  Avarage score: %zu\n", score_sum / iterations );
		printf( "\n     Best time : %5.2f secs.\n", time_best / 1e9 );
		printf( "     Best score: %zu\n", score_best );
		return true;
	}
}; // class task

} // namespace ts
