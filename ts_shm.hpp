#pragma once

namespace ts {

// Note: shm objects resides in /run/shm
class shm {
	bool		m_is_valid	= false;
	std::string	m_name;
	size_t		m_size_reserved;
	int			m_fd;
	void *		m_ptr;

public:
	shm( const std::string & name, size_t size_reserved )
		: m_name( name )
		, m_size_reserved( size_reserved )
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
			shm_unlink( m_name.c_str() );
		}
	}

	constexpr explicit operator bool () const { return m_is_valid; }

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
		size_t cur = lseek( m_fd, 0, SEEK_CUR );
		size_t end = lseek( m_fd, 0, SEEK_END );
		lseek( m_fd, cur, SEEK_SET );
		return end;
	}


	template <typename T>
	constexpr memory_view <T> mem_view() {
		assert( m_size_reserved % sizeof( T ) == 0 );
		return memory_view<T>( static_cast<T *>( m_ptr ), m_size_reserved / sizeof( T ) );
	}
}; // class shm

} // namespace ts
