#pragma once

namespace ts {

class scoped_fd_subst {
	int	m_old;
	int	m_new;

public:
	scoped_fd_subst( int old_fd, int subst_with )
		: m_old( old_fd )
		, m_new( dup( old_fd ) )
	{
		//m_new = dup( old_fd );
		if ( m_new == -1 ) {
			perror( "dup()" );
			return;
		}

		close( old_fd );
		if ( dup2( subst_with, old_fd ) == -1 ) {
			perror( "dup2()" );
			return;
		}

		//fprintf( stderr, " new_fd: %2d, old_fd: %2d, subst_with: %2d\n", m_new, m_old, subst_with );
	}

	virtual ~scoped_fd_subst() {
		if ( dup2( m_new, m_old ) == -1 ) {
			perror( "dup2()" );
			return;
		}
		close( m_new );
	}

	constexpr explicit operator bool () const { return m_new != -1; }
}; // class scoped_fd_subst

} // namespace ts
