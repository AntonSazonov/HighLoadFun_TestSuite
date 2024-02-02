#include <cstdio>
#include <cstdlib>

#include <sched.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
#if 1
	int run_on = 0;//sched_getcpu() / 2;

	cpu_set_t cpu_set;
	CPU_ZERO( &cpu_set );
	CPU_SET( run_on, &cpu_set );
	if ( sched_setaffinity( 0/*getpid()*/, sizeof( cpu_set ), &cpu_set ) == -1 ) {
		perror( "sched_setaffinity()" );
	}
#endif

	printf( " sched_getcpu(): %d\n", sched_getcpu() );
//	printf( "           CPUs: %ld avail.\n", sysconf( _SC_NPROC_ONLN ) );
	printf( "           CPUs: %ld avail.\n", sysconf( _SC_NPROCESSORS_ONLN ) );
	printf( "         nprocs: %d avail.\n", get_nprocs() );

#if 1
	pid_t pid = fork();
	switch ( pid ) {
		case -1:
			perror( "fork()" );
			break;

		case 0: {
			fprintf( stderr, "  child: %d\n", sched_getcpu() );
			system( "./child/main" );
			//if ( execv( "./child/main", nullptr ) == -1 ) {

			const char * name = "./child/main";
			if ( execlp( name, name ) == -1 ) {
				perror( "execv()" );
			}
			exit( EXIT_SUCCESS );
			//break;
		}

		default: {
			fprintf( stderr, " parent: %d\n", sched_getcpu() );

			int status;
			waitpid( pid, &status, 0 );
			exit( EXIT_SUCCESS );
			//break;
		}
	}
#endif

	//system( "./child/main" );

	// int execv(const char *path, char *const argv[]);
	// int execvp(const char *file, char *const argv[]);

	return 0;
}
