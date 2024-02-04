#include <cstdio>
#include <cstdint>
#include <cstring>

#include <string>
#include <filesystem>
//#include <fstream>
#include <optional>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
//#include <linux/mman.h>

#include <sys/resource.h>

#if 0
// For stderr, append: 2>&1
bool exec( const char * command ) {
	FILE * fp = popen( command, "r" );
	if ( !fp ) {
		perror( "popen()" );
		return false;
	}

	char str[1024] = {};
	while ( fgets( str, 1024, fp ) ) {
		fprintf( stderr, "%s", str ); // %.*s
	}

	pclose( fp );
	return true;
}
#endif

bool is_file_exists( const std::string & filename ) {
	return std::filesystem::exists( std::filesystem::path{ filename } );
}

bool check_input_data( int test_num ) {
	char test_num_sz[3];
	sprintf( test_num_sz, "%02d", test_num );
	return is_file_exists( std::string{ "/mnt/highload/" } + test_num_sz + "_data" );
	//return std::filesystem::exists( std::filesystem::path{ test_input_data } );
}


std::optional <std::string> find_test( int test_num ) {

	char test_num_sz[3];
	sprintf( test_num_sz, "%02d", test_num );

	// Find test path...
	const std::filesystem::path sandbox{ ".." };
	for ( auto const & dir_entry : std::filesystem::directory_iterator{ sandbox } ) {
		if ( dir_entry.is_directory() ) {
			std::string e = dir_entry.path().stem().string();
			if ( !e.compare( 0/*pos*/, 2/*size*/, test_num_sz ) ) {
				return e;//break;
			}
		}
	}
	return std::nullopt;
}

int main( int argc, const char * args[] ) {
	if ( argc != 2 ) {
		fprintf( stderr, "Use: %s test_number\n", args[0] );
		return 1;
	}

	int test_num = std::stoi( std::string{ args[1] } );
	if ( test_num < 0 || test_num > 99 ) {
		fprintf( stderr, "Test # must be in interval [0;99]\n" );
		return 2;
	}


	std::optional <std::string> test_path = find_test( test_num );
	if ( !test_path ) {
		fprintf( stderr, " * Test %d not found.\n", test_num );
		return 3;
	}

	printf( " * Testing: %s ...\n", test_path.value().c_str() );

	if ( !check_input_data( test_num ) ) {
		printf( " * Input data for test not exists. Generating...\n" );

		printf( " * TODO\n" );
		//return 3;
	}

	// Run te
	//std::string cmd{ std::string{ "../" + test_path.value() + "/main" }.c_str()
//	std::string cmd{ "../" + test_path.value() + "/main > z_stdout" };
	std::string cmd{ "./_t.sh" };

	int r = std::system( cmd.c_str() );
	if ( r == -1 ) {
		fprintf( stderr, "\nCan't execute test: %s\n", cmd.c_str() );
	}
	printf( "\nExit status: %d\n\n", WEXITSTATUS( r ) );

	rusage res;
	getrusage( RUSAGE_CHILDREN, &res );

	uint32_t usr = res.ru_utime.tv_sec * 1'000'000 + res.ru_utime.tv_usec;
	uint32_t sys = res.ru_stime.tv_sec * 1'000'000 + res.ru_stime.tv_usec;

	printf( "    user: %u\n", usr );
	printf( "     sys: %u\n", sys );
	printf( "   score: %u\n\n", (usr + sys) / 10 );

	return 0;
}
