#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
//#include <linux/mman.h>

int main() {
	int fd = shm_open( "/my_stdin", O_CREAT | O_RDWR | O_TRUNC, 0/*mode*/ );
	if ( fd == -1 ) {
		perror( "shm_open()" );
		return 1;
	}

	printf( " fd: %d\n", fd );

	size_t size = 100;//'000'000;
	if ( ftruncate( fd, size ) == -1 ) {
		perror( "ftruncate()" );
		return 2;
	}

	char * p_shm = (char *)mmap( 0, size, PROT_READ | PROT_WRITE, MAP_SHARED /*| MAP_NONBLOCK*/, fd, 0 );
	if ( p_shm == MAP_FAILED ) {
		perror( "mmap()" );
		return 3;
	}




	int fd_o = shm_open( "/my_stdout", O_CREAT | O_RDWR | O_TRUNC, 0/*mode*/ );
	if ( fd_o == -1 ) {
		perror( "shm_open()" );
		return 1;
	}

	printf( "fdo: %d\n", fd_o );

	if ( ftruncate( fd_o, size ) == -1 ) {
		perror( "ftruncate()" );
		return 2;
	}

	char * p_shm_o = (char *)mmap( 0, size, PROT_READ | PROT_WRITE, MAP_SHARED /*| MAP_NONBLOCK*/, fd_o, 0 );
	if ( p_shm_o == MAP_FAILED ) {
		perror( "mmap()" );
		return 3;
	}


	strcpy( p_shm, "echo Hello!" );
//	strcpy( p_shm, "echo Hello! 1>&2" );

	//int shm_unlink(const char *name);

//	system( "echo \"Test\" | tee" );

	int old_stdin  = dup( STDIN_FILENO );
	int old_stdout = dup( STDOUT_FILENO );


	close( STDIN_FILENO );
	if ( dup2( fd, STDIN_FILENO ) == -1 ) {
		perror( "dup2()" );
		return 4;
	}

	close( STDOUT_FILENO );
	if ( dup2( fd_o, STDOUT_FILENO ) == -1 ) {
		perror( "dup2()" );
		return 4;
	}

	system( "bash" );

//	char * const args[] = { "bash", NULL };
//	execvp( "bash", args );

	if ( dup2( old_stdin, STDIN_FILENO ) == -1 ) {
		perror( "dup2()" );
		return 4;
	}
	close( old_stdin );

	if ( dup2( old_stdout, STDOUT_FILENO ) == -1 ) {
		perror( "dup2()" );
		return 4;
	}
	close( old_stdout );

	fprintf( stdout, "[%s]\n", p_shm_o );
	fprintf( stderr, "[%s]\n", p_shm_o );

	return 0;
}
