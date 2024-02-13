#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <bits/stdc++.h>
#include <immintrin.h>

constexpr size_t	n_pixels	= 125'000'000;
constexpr size_t	n_bytes		= n_pixels * 4lu;

constexpr static std::size_t huge_page_size = 1 << 21; // 2 MiB

constexpr size_t round_to_huge_page_size( size_t n ) {
	return (((n - 1) / huge_page_size) + 1) * huge_page_size;
}

int main() {
//	const uint8_t * pi = (const uint8_t *)mmap( (void *)0x8000000000000000UL, round_to_huge_page_size( n_bytes ), PROT_READ, MAP_FIXED | MAP_PRIVATE | MAP_POPULATE | MAP_HUGETLB | (29 << MAP_HUGE_SHIFT) | MAP_NONBLOCK, STDIN_FILENO, 0 );
//	const char * p_in = (const char *)mmap( (void *)0x10000000, size, PROT_READ, MAP_PRIVATE | MAP_POPULATE | MAP_FIXED | MAP_HUGE_256MB, STDIN_FILENO, 0 );

	const void * p_in = mmap( (void *)0x100000000, n_bytes, PROT_READ, MAP_FIXED | MAP_PRIVATE | MAP_POPULATE | MAP_NONBLOCK, STDIN_FILENO, 0 );
	if ( p_in == MAP_FAILED ) {
		perror( "mmap( stdin )" );
		return 1;
	}
	madvise( (void *)p_in, n_bytes, MADV_SEQUENTIAL | MADV_WILLNEED /*| MADV_POPULATE_READ*/ );

	posix_fallocate( STDOUT_FILENO, 0, n_pixels );
	void * p_out = mmap( (void *)0x400000000, n_pixels, PROT_WRITE, MAP_FIXED | MAP_SHARED | MAP_NONBLOCK, STDOUT_FILENO, 0 );
	if ( p_out == MAP_FAILED ) {
		perror( "mmap( stdout )" );
		return 1;
	}


	const uint32_t *	pi	= (const uint32_t *)p_in;
	uint8_t *			po	= (uint8_t *)p_out;

	size_t n = n_pixels;
	while ( n-- ) {
		*po++ = *pi++ >> 16 & 0xff;
	}

#if 0
	const __m256i shuf_bb = _mm256_setr_epi8(
		2, 6, 10, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// lane 1
		2, 6, 10, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );	// lane 2

	const __m256i perm_04 = _mm256_setr_epi32( 0, 4, 1, 1, 1, 1, 1, 1 );


	__asm__ __volatile__ ( "# LLVM-MCA-BEGIN foo" ::: "memory" );

	const __m256i * pi = (const __m256i *)p_in;
//	uint32_t * po = (uint32_t *)p_out;
	uint64_t * po = (uint64_t *)p_out;

#if 0
		xor			ebp, ebp
		vmovaps		xmm0, xmmword ptr [rsp + 16]
		vmovaps		xmm1, xmmword ptr [rsp]
		vmovntps	xmmword ptr [rbx + rbp + 16], xmm0
		vmovntps	xmmword ptr [rbx + rbp], xmm1
		add			rbp, 4
		cmp			rbp, 62500000
		jne			.LBB0_5


	#NO_APP
	xor	ebp, ebp
	vmovdqa	xmm0, xmmword ptr [rip + .LCPI0_0] # xmm0 = <2,6,10,14,u,u,u,u,u,u,u,u,u,u,u,u>
	.p2align	4, 0x90
.LBB0_5:                                # =>This Inner Loop Header: Depth=1
	vmovdqa	xmm1, xmmword ptr [rbx + 4*rbp]
	vmovdqa	xmm2, xmmword ptr [rbx + 4*rbp + 16]
	vpshufb	xmm2, xmm2, xmm0
	vpshufb	xmm1, xmm1, xmm0
	vpunpckldq	xmm1, xmm1, xmm2        # xmm1 = xmm1[0],xmm2[0],xmm1[1],xmm2[1]
	vmovq	qword ptr [r15 + rbp], xmm1
	add	rbp, 8
	cmp	rbp, 125000000
	jne	.LBB0_5
# %bb.6:
	#APP
#endif

	size_t n = n_pixels / 8;
	while ( n-- ) {
#if 1
		__m256i r = _mm256_shuffle_epi8( _mm256_stream_load_si256( pi ), shuf_bb );
		r = _mm256_permutevar8x32_epi32( r, perm_04 );
		*po++ = _mm256_extract_epi64( r, 0 );
		pi++;
#endif


#if 0
		const __m256i v1 = _mm256_shuffle_epi8( _mm256_stream_load_si256( pi     ), mask );
		const __m256i v2 = _mm256_shuffle_epi8( _mm256_stream_load_si256( pi + 1 ), mask );
		const __m256i v3 = _mm256_shuffle_epi8( _mm256_stream_load_si256( pi + 2 ), mask );
		const __m256i v4 = _mm256_shuffle_epi8( _mm256_stream_load_si256( pi + 3 ), mask );

		volatile __m128i a, b;
		__m256i xx = _mm256_set_m128i( a, b );

		// vmovntps	xmmword ptr [rbx + rbp], xmm1
		// vmovntdq m256, ymm
		_mm256_stream_si256( (__m256i *)po, xx );

//		*po++ = _mm256_extract_epi32( r, 0 );
//		*po++ = _mm256_extract_epi32( r, 4 );


		// vinsertf128 ymm, ymm, xmm, imm8
		//__m256i _mm256_set_m128i (__m128i hi, __m128i lo)


		//_mm256_stream_si256
		pi += 4;
		po += 1;
#endif
	}
	__asm__ __volatile__ ( "# LLVM-MCA-END" ::: "memory" );
#endif



	ftruncate( STDOUT_FILENO, n_pixels );
	msync( p_out, n_pixels, MS_SYNC );

	return 0;
}
