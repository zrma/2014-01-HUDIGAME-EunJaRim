#pragma once

#include <cassert>

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
			m_Instance = new T;
		}
		return m_Instance;
	}

	static void Release( void )
	{
		SafeDelete( m_Instance );
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
inline void LogT( const Types& ...args )
{
#ifdef _DEBUG
	char buf[100];
	sprintf_s( buf, sizeof( buf ), args... );
	printf_s( "[TRACE]%s", buf );
#endif // _DEBUG
}

///# 저번에 설명했다.. 함수를 따로 만들지말고.. config에 따라 쫙~ 변하게 ㅋㅋ
template <typename ... Types >
inline void LogD( const Types& ...args )
{
#ifdef _DEBUG
	char buf[100];
	sprintf_s( buf, sizeof( buf ), args... );
	printf_s( "[DEBUG]%s", buf );
#endif // _DEBUG
}
template <typename ... Types >
inline void LogI( const Types& ...args )
{
#ifdef _DEBUG
	char buf[100];
	sprintf_s( buf, sizeof( buf ), args... );
	printf_s( "[INFO]%s", buf );
#endif // _DEBUG
}
template <typename ... Types >
inline void LogW( const Types& ...args )
{
#ifdef _DEBUG
	char buf[100];
	sprintf_s( buf, sizeof( buf ), args... );
	printf_s( "[WARN]%s", buf );
#endif // _DEBUG
}
template <typename ... Types >
inline void LogE( const Types& ...args )
{
#ifdef _DEBUG
	char buf[100];
	sprintf_s( buf, sizeof( buf ), args... );
	printf_s( "[ERROR]%s", buf );
#endif // _DEBUG
}
template <typename ... Types >
inline void LogF( const Types& ...args )
{
#ifdef _DEBUG
	char buf[100];
	sprintf_s( buf, sizeof( buf ), args... );
	printf_s( "[FATAL]%s", buf );
#endif // _DEBUG
}


