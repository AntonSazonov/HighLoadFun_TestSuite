#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <bits/stdc++.h>
#include <immintrin.h>

constexpr size_t	n_pixels	= 125'000'000;
constexpr size_t	n_bytes		= n_pixels * 4lu;

int main() {
	const void * p_in = mmap( (void *)0x100000000, n_bytes, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED /*| MAP_POPULATE | MAP_NONBLOCK*/, STDIN_FILENO, 0 );
	if ( p_in == MAP_FAILED ) {
		perror( "mmap( stdin )" );
		return 1;
	}

	posix_fallocate( STDOUT_FILENO, 0, n_pixels );
	void * const p_out = mmap( (void *)0x400000000, n_pixels, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED /*MAP_PRIVATE | MAP_NONBLOCK*/, STDOUT_FILENO, 0 );
	if ( p_out == MAP_FAILED ) {
		perror( "mmap( stdout )" );
		return 1;
	}


	uint8_t * ps = (uint8_t *)p_in + 2;
	uint8_t * pd = (uint8_t *)p_out;

	int n = n_pixels;
	while ( n-- ) {
		*pd++ = *ps;
		ps += 4;
	}

	ftruncate( STDOUT_FILENO, n_pixels );
	msync( p_out, n_pixels, MS_SYNC );

	return 0;
}
