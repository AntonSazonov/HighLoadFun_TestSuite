
#include "ts_pch.hpp"

#include "ts_scoped_fd_subst.hpp"
#include "ts_memory_view.hpp"
#include "ts_shm.hpp"
#include "ts_timer.hpp"
#include "ts_task.hpp"
#include "ts_task_01.hpp"
#include "ts_task_02.hpp"

int main( int argc, const char * args[] ) {

	if ( argc < 3 ) {
		printf( "Use: %s <task_number> <prog_to_test> [iterations]\n", args[0] );
		return 1;
	}

#if 1
	int run_on = 0;//sched_getcpu() / 2;

	cpu_set_t cpu_set;
	CPU_ZERO( &cpu_set );
	CPU_SET( run_on, &cpu_set );
	if ( sched_setaffinity( 0/*getpid()*/, sizeof( cpu_set ), &cpu_set ) == -1 ) {
		perror( "sched_setaffinity()" );
	}
#endif

#if 0
//	printf( "    GetCPUCount: %d\n", GetCPUCount() );
	printf( " sched_getcpu(): %d\n", sched_getcpu() );
//	printf( "           CPUs: %ld avail.\n", sysconf( _SC_NPROC_ONLN ) );
	printf( "           CPUs: %ld avail.\n", sysconf( _SC_NPROCESSORS_ONLN ) );
	printf( "         nprocs: %d avail.\n", get_nprocs() );
#endif


	int task_id = std::atoi( args[1] );
	if ( !task_id ) {
		fprintf( stderr, "Invalid task #: '%s'.\n", args[1] );
		return 1;
	}

	{
		struct stat sb;
		if ( stat( args[2], &sb ) == -1 ) {
			fprintf( stderr, "No such executable: '%s'.\n", args[2] );
			return 1;
		}
	}
	std::string	executable = args[2];

	int interations	= 3;
	if ( argc == 4 ) {
		interations = std::atoi( args[3] );
		if ( !interations ) {
			fprintf( stderr, "Invalid number of iterations: '%s'\n", args[3] );
			return 1;
		}
	}

	std::unique_ptr <ts::task> p_task;
	switch ( task_id ) {
		case 1: p_task = std::unique_ptr<ts::task>( new (std::nothrow) ts::task_01 ); break;
		case 2: p_task = std::unique_ptr<ts::task>( new (std::nothrow) ts::task_02 ); break;

		default:
			printf( " No such task %d.\n", task_id );
			break;
	}

	if ( !p_task || !*p_task ) {
		fprintf( stderr, " !p_task || !*p_task.\n" );
		return 1;
	}

	if ( !p_task->run( executable, interations ) ) {
		fprintf( stderr, "\n Task test failed.\n" );
		return 2;
	}

	return 0;
}
