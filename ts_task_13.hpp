#pragma once

namespace ts {

#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)			(((x) ^ (y)) ^ (z))
#define H2(x, y, z)			((x) ^ ((y) ^ (z)))
#define I(x, y, z)			((y) ^ ((x) | ~(z)))

#define STEP( f, a, b, c, d, x, t, s )						\
	(a) += f((b), (c), (d)) + (((uint32_t *)ptr)[x]) + (t);	\
	(a) = (((a) << (s)) | ((a) >> (32 - (s))));				\
	(a) += (b);

class task_13 : public task {
	static constexpr size_t	t_divisor	= 1'000;
	static constexpr size_t	t_in_size	= 250'000'000;
	static constexpr size_t	t_out_size	= 32;

	typedef struct {
		uint32_t	lo, hi;
		uint32_t	a, b, c, d;
		uint8_t		buffer[64];
	} MD5_CTX;

const uint8_t * body( MD5_CTX * ctx, const uint8_t * ptr, uint32_t size ) {

	uint32_t a = ctx->a;
	uint32_t b = ctx->b;
	uint32_t c = ctx->c;
	uint32_t d = ctx->d;

	do {
		uint32_t saved_a = a;
		uint32_t saved_b = b;
		uint32_t saved_c = c;
		uint32_t saved_d = d;

		/* Round 1 */
		STEP(F, a, b, c, d,  0, 0xd76aa478,  7)
		STEP(F, d, a, b, c,  1, 0xe8c7b756, 12)
		STEP(F, c, d, a, b,  2, 0x242070db, 17)
		STEP(F, b, c, d, a,  3, 0xc1bdceee, 22)

		STEP(F, a, b, c, d,  4, 0xf57c0faf,  7)
		STEP(F, d, a, b, c,  5, 0x4787c62a, 12)
		STEP(F, c, d, a, b,  6, 0xa8304613, 17)
		STEP(F, b, c, d, a,  7, 0xfd469501, 22)

		STEP(F, a, b, c, d,  8, 0x698098d8,  7)
		STEP(F, d, a, b, c,  9, 0x8b44f7af, 12)
		STEP(F, c, d, a, b, 10, 0xffff5bb1, 17)
		STEP(F, b, c, d, a, 11, 0x895cd7be, 22)

		STEP(F, a, b, c, d, 12, 0x6b901122,  7)
		STEP(F, d, a, b, c, 13, 0xfd987193, 12)
		STEP(F, c, d, a, b, 14, 0xa679438e, 17)
		STEP(F, b, c, d, a, 15, 0x49b40821, 22)

		/* Round 2 */
		STEP(G, a, b, c, d,  1, 0xf61e2562,  5)
		STEP(G, d, a, b, c,  6, 0xc040b340,  9)
		STEP(G, c, d, a, b, 11, 0x265e5a51, 14)
		STEP(G, b, c, d, a,  0, 0xe9b6c7aa, 20)
		STEP(G, a, b, c, d,  5, 0xd62f105d,  5)
		STEP(G, d, a, b, c, 10, 0x02441453,  9)
		STEP(G, c, d, a, b, 15, 0xd8a1e681, 14)
		STEP(G, b, c, d, a,  4, 0xe7d3fbc8, 20)
		STEP(G, a, b, c, d,  9, 0x21e1cde6,  5)
		STEP(G, d, a, b, c, 14, 0xc33707d6,  9)
		STEP(G, c, d, a, b,  3, 0xf4d50d87, 14)
		STEP(G, b, c, d, a,  8, 0x455a14ed, 20)
		STEP(G, a, b, c, d, 13, 0xa9e3e905,  5)
		STEP(G, d, a, b, c,  2, 0xfcefa3f8,  9)
		STEP(G, c, d, a, b,  7, 0x676f02d9, 14)
		STEP(G, b, c, d, a, 12, 0x8d2a4c8a, 20)

		/* Round 3 */
		STEP( H, a, b, c, d,  5, 0xfffa3942,  4)
		STEP(H2, d, a, b, c,  8, 0x8771f681, 11)
		STEP( H, c, d, a, b, 11, 0x6d9d6122, 16)
		STEP(H2, b, c, d, a, 14, 0xfde5380c, 23)
		STEP( H, a, b, c, d,  1, 0xa4beea44,  4)
		STEP(H2, d, a, b, c,  4, 0x4bdecfa9, 11)
		STEP( H, c, d, a, b,  7, 0xf6bb4b60, 16)
		STEP(H2, b, c, d, a, 10, 0xbebfbc70, 23)
		STEP( H, a, b, c, d, 13, 0x289b7ec6,  4)
		STEP(H2, d, a, b, c,  0, 0xeaa127fa, 11)
		STEP( H, c, d, a, b,  3, 0xd4ef3085, 16)
		STEP(H2, b, c, d, a,  6, 0x04881d05, 23)
		STEP( H, a, b, c, d,  9, 0xd9d4d039,  4)
		STEP(H2, d, a, b, c, 12, 0xe6db99e5, 11)
		STEP( H, c, d, a, b, 15, 0x1fa27cf8, 16)
		STEP(H2, b, c, d, a,  2, 0xc4ac5665, 23)

		/* Round 4 */
		STEP(I, a, b, c, d,  0, 0xf4292244, 6)
		STEP(I, d, a, b, c,  7, 0x432aff97, 10)
		STEP(I, c, d, a, b, 14, 0xab9423a7, 15)
		STEP(I, b, c, d, a,  5, 0xfc93a039, 21)
		STEP(I, a, b, c, d, 12, 0x655b59c3, 6)
		STEP(I, d, a, b, c,  3, 0x8f0ccc92, 10)
		STEP(I, c, d, a, b, 10, 0xffeff47d, 15)
		STEP(I, b, c, d, a,  1, 0x85845dd1, 21)
		STEP(I, a, b, c, d,  8, 0x6fa87e4f, 6)
		STEP(I, d, a, b, c, 15, 0xfe2ce6e0, 10)
		STEP(I, c, d, a, b,  6, 0xa3014314, 15)
		STEP(I, b, c, d, a, 13, 0x4e0811a1, 21)
		STEP(I, a, b, c, d,  4, 0xf7537e82, 6)
		STEP(I, d, a, b, c, 11, 0xbd3af235, 10)
		STEP(I, c, d, a, b,  2, 0x2ad7d2bb, 15)
		STEP(I, b, c, d, a,  9, 0xeb86d391, 21)

		a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} while ( size -= 64 );

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	return ptr;
}

void MD5_Init( MD5_CTX * ctx ) {
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

void MD5_Update( MD5_CTX * ctx, const uint8_t * data, uint32_t size ) {
	uint32_t saved_lo = ctx->lo;
	if ( (ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo ) ctx->hi++;
	ctx->hi += size >> 29;
	uint32_t used = saved_lo & 0x3f;
	if ( used ) {
		uint32_t available = 64 - used;
		if ( size < available ) {
			memcpy( &ctx->buffer[used], data, size );
			return;
		}
		memcpy( &ctx->buffer[used], data, available );
		//data = (const uint8_t *)data + available;
		data += available;
		size -= available;
		body( ctx, ctx->buffer, 64 );
	}

	if ( size >= 64 ) {
		data = body( ctx, data, size & ~(uint32_t)0x3f );
		size &= 0x3f;
	}

	memcpy( ctx->buffer, data, size );
}

#define OUT(dst, src) \
	(dst)[0] = (uint8_t)(src); \
	(dst)[1] = (uint8_t)((src) >> 8); \
	(dst)[2] = (uint8_t)((src) >> 16); \
	(dst)[3] = (uint8_t)((src) >> 24);

void MD5_Final( uint8_t * result, MD5_CTX * ctx ) {

	uint32_t used = ctx->lo & 0x3f;

	ctx->buffer[used++] = 0x80;

	uint32_t available = 64 - used;

	if ( available < 8 ) {
		memset( &ctx->buffer[used], 0, available );
		body( ctx, ctx->buffer, 64 );
		used = 0;
		available = 64;
	}

	memset( &ctx->buffer[used], 0, available - 8 );

	ctx->lo <<= 3;
	OUT( &ctx->buffer[56], ctx->lo )
	OUT( &ctx->buffer[60], ctx->hi )

	body( ctx, ctx->buffer, 64 );

	OUT( &result[ 0], ctx->a )
	OUT( &result[ 4], ctx->b )
	OUT( &result[ 8], ctx->c )
	OUT( &result[12], ctx->d )

	memset( ctx, 0, sizeof( *ctx ) );
}


public:
	task_13() : task( "MD5", t_divisor, t_in_size, t_out_size ) {}

	bool generate_input( random_generator_t & generator ) override {
		int_distribution_t <uint32_t> dist;

		size_t		n = t_in_size / sizeof( uint32_t );
		uint32_t *	p = m_stdin.data<uint32_t>();

		auto gen = [&]{ return dist( generator ); };

		// Generate randoms...
		std::generate( p, p + n, gen );

		MD5_CTX ctx;
		MD5_Init( &ctx );
		MD5_Update( &ctx, (uint8_t *)p, t_in_size );

		uint8_t hash[16];
		MD5_Final( hash, &ctx );

		char hash_str[33];
		for ( int i = 0; i < 16; i++ ) {
			sprintf( &hash_str[i * 2], "%02x", hash[i] );
		}

		std::memcpy( m_expected.data<char>(), hash_str, 32 );

		return true;
	}
}; // class task_13

#undef F
#undef G
#undef H
#undef H2
#undef I
#undef STEP

} // namespace ts
