#pragma once

namespace ts {

template <typename T>
class memory_view {
	T *		m_data{};
	size_t	m_size{};

public:
	constexpr memory_view( T * data, size_t size )
		: m_data( data )
		, m_size( size ) {}

	constexpr size_t	size() const { return m_size; }
	constexpr const T *	data() const { return m_data; }
	constexpr T *		data()       { return m_data; }

	constexpr const T & operator [] ( ptrdiff_t index ) const { return m_data[index]; }
	constexpr       T & operator [] ( ptrdiff_t index )       { return m_data[index]; }

	//bool is_equal( memory_view <T> other ) {
	constexpr bool operator == ( const memory_view <T> & other ) const {
		return m_size == other.size() &&
			!std::memcmp(
				static_cast<const void *>( m_data ),
				static_cast<const void *>( other.data() ),
				m_size * sizeof( T ) );
	}
}; // class memory_view

} // namespace ts
