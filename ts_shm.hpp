#pragma once

namespace ts {

// Note: shm objects resides in /run/shm
class shm {
	bool		m_is_valid	= false;
	std::string	m_name;
	size_t		m_size_reserved;
//	size_t		m_size_actual;
	int			m_fd;
	void *		m_ptr;

public:
	shm( const std::string & name, size_t size_reserved )
		: m_name( name )
		, m_size_reserved( size_reserved )
//		, m_size_actual( size_reserved )
	{
		m_fd = shm_open( m_name.c_str(), O_CREAT | O_TRUNC/*O_EXCL*/ | O_RDWR, 0/*mode*/ );
		if ( m_fd == -1 ) {
			perror( "shm_open()" );
			return;
		}

		if ( ftruncate( m_fd, m_size_reserved ) == -1 ) {
			perror( "ftruncate()" );
			return;
		}

		m_ptr = mmap( 0, m_size_reserved, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_NONBLOCK, m_fd, 0 );
		if ( m_ptr == MAP_FAILED ) {
			perror( "mmap()" );
			return;
		}

		//fprintf( stderr, "lseek: %zu\n", lseek( m_fd, 0, SEEK_END ) );
		//lseek( m_fd, 0, SEEK_SET );

		m_is_valid = true;
	}

	virtual ~shm() {
		if ( operator bool () ) {
			munmap( m_ptr, m_size_reserved );
			shm_unlink( m_name.c_str() );
		}
	}

	constexpr explicit operator bool () const { return m_is_valid; }

	void reset() {
//		m_size_actual = size;
		if ( ftruncate( m_fd, m_size_reserved ) == -1 ) {
			perror( "ftruncate()" );
		}
	}

	void resize( size_t size ) {
//		m_size_actual = size;
		if ( ftruncate( m_fd, size ) == -1 ) {
			perror( "ftruncate()" );
		}
	}

	std::string			name()	const { return m_name; }
	constexpr size_t	size_reserved()	const { return m_size_reserved; }
	constexpr int		fd()	const { return m_fd; }
	constexpr void *	data()	const { return m_ptr; }

	template <typename T>
	constexpr T *		data()	const { return static_cast<T *>( m_ptr ); }

	void rewind() const {
		lseek( m_fd, 0, SEEK_SET );
	}

	// Real size
	size_t size() const {
//		return m_size_actual;
#if 1
		size_t cur = lseek( m_fd, 0, SEEK_CUR );
		size_t end = lseek( m_fd, 0, SEEK_END );
		lseek( m_fd, cur, SEEK_SET );
		return end;
#endif
	}


	template <typename T>
	constexpr memory_view <T> mem_view() {
		assert( m_size_reserved % sizeof( T ) == 0 );
		return memory_view<T>( static_cast<T *>( m_ptr ), m_size_reserved / sizeof( T ) );
	}
}; // class shm

} // namespace ts
