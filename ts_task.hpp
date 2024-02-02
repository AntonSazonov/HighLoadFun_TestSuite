#pragma once

namespace ts {

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
		, m_stdin( "/ts_stdin", stdin_max_size )
		, m_stdout( "/ts_stdout", stdout_max_size )
		, m_expected( "/ts_expected", stdout_max_size ) {}

	virtual ~task() = default;

	constexpr explicit operator bool () const {
		return m_stdin && m_stdout && m_expected;
	}

	//std::string name()	const { return m_name; }
	//int score_divisor()	const { return m_score_divisor; }

	virtual void generate_input( std::mt19937_64 & ) = 0;

	virtual bool check_result() {

		// chech only N first chars (m_expected.size())
		return m_stdout.mem_view<char>() == m_expected.mem_view<char>();
	}

	bool run( const std::string & executable, int iterations = 3 ) {

		fprintf( stderr, " # Testing '%s', %d iteration%s...\n", m_name.c_str(), iterations, iterations > 1 ? "s" : "" );

		std::random_device	rd;
		std::seed_seq		seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
		std::mt19937_64		random_generator( seed );

		ts::timer	timer_child;
		uint64_t	score_sum = 0;

		for ( int i = 0; i < iterations; i++ ) {
			m_stdout.rewind();

			fprintf( stderr, "\n Iteration #%2d:\n", i + 1 );
			fprintf( stderr, "   Generating input data..." );

			ts::timer <RUSAGE_SELF> timer_self;
			generate_input( random_generator );
			fprintf( stderr, " %.2f secs.\n", timer_self.us() / 1e6 );

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

			uint64_t time_ns	= timer_child.ns();
			uint64_t score		= time_ns / m_score_divisor;
			score_sum += score;

			bool is_correct = check_result();
			if ( is_correct ) {
				printf( "         Result: passed\n" );
			} else {
				printf( "         Result: not passed\n" );
				printf( "       Expected: [%.*s]\n", int(m_expected.mem_view<char>().size()), m_expected.mem_view<char>().data() );
				printf( "            Got: [%.*s]\n", int(m_stdout  .mem_view<char>().size()), m_stdout  .mem_view<char>().data() );
			}
			printf( "           Time: %5.2f secs.\n", time_ns / 1e9);
			printf( "          Score: %zu\n", score );

			if ( !is_correct ) return false;
		}
		printf( "\n  Avarage score: %zu\n", score_sum / iterations );
		return true;
	}
}; // class task

} // namespace ts
