#pragma  once

#include <cassert>
#define ALIGNMENT	16

template <class T>
class Singleton
{
protected:
	Singleton( void ) {}
	virtual ~Singleton( void ) {}
public:
	static T* GetInstance( void )
	{
		if ( !m_Instance )
		{
			m_Instance = (T*)_aligned_malloc( sizeof( T ), ALIGNMENT );
		}
		return m_Instance;
	}
	
	static void Release( void )
	{
		if ( m_Instance )
		{
			_aligned_free( m_Instance );
		}
	}

private:
	static T*	m_Instance;
};
template <typename T>
T* Singleton<T>::m_Instance = nullptr;


template <typename T>
inline void SafeRelease( T* &p )
{
	if ( p != nullptr )
	{
		p->Release();
		p = nullptr;
	}
}

template <typename T>
inline void SafeDelete( T* &p )
{
	if ( p != nullptr )
	{
		delete p;
		p = nullptr;
	}
}

template <typename ... Types >
inline void Log( const Types& ...args )
{
#ifdef _DEBUG
	printf_s( args... );
#endif // _DEBUG
}