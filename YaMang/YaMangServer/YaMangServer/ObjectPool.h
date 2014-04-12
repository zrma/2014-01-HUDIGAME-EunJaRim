#pragma once

#include "Exception.h"

/// http://blog.secmem.org/104 참조
template <class TOBJECT, int ALLOC_COUNT=10>
class ObjectPool
{
public:

	// C++ 연산자 오버로딩
	// new 연산자 오버로딩
	static void* operator new( size_t objSize )
	{
		// 초기 생성 or 꽉 찼을 때
		if ( !m_FreeList )
		{
			// TOBJECT의 10칸 배열을 미리 동적 생성 해 둠
			m_FreeList = new uint8_t[sizeof(TOBJECT)*ALLOC_COUNT];

			// 엄밀히 따지면 mFreeList는 uint8_t(unsigned char) 배열이므로
			// 2중 배열 형태로 만들 필요가 있음
			uint8_t* pNext = m_FreeList;
			// mFreeList의 포인터(현재는 배열 시작위치)를 mFreeList의 이중 포인터 형태로 형 변환 해서 넣어둠
			uint8_t** ppCurr = reinterpret_cast<uint8_t**>( m_FreeList );

			/// OBJECT의 크기가 반드시 포인터 크기보다 커야 한다
			// 배열 안에 미리 다음 위치의 포인터를 찍어 둠
			// 그러려면 OBJECT의 크기가 반드시 포인터 크기보다 커야 됨
			// 그래야 OBJECT가 들어갈 공간에 미리 포인터를 찍어 놓을 수 있음
			for ( int i = 0; i < ALLOC_COUNT - 1; ++i )
			{
				// pNext가 다음 OBJECT 칸을 가리킨다
				pNext += sizeof( TOBJECT );

				// 2중 포인터로 mFreeList의 위치를 ppCurr에 넣어두었으니
				// *ppCurr로 해당 위치에 pNext를 넣게 된다
				// (결과적으로 다음 위치 주소를 해당 배열에 넣어둠)
				*ppCurr = pNext;

				// 그리고 ppCurr은 다음 배열 위치의 주소로
				ppCurr = reinterpret_cast<uint8_t**>( pNext );
			}

			*ppCurr = 0; ///< 마지막은 0으로 표시

			// 마지막에 다다르게 되면 mFreeList의 값이 0이 되어서 false, !mFreeList = true
			// 위의 if문에 검출 되어서 배열을 추가 할당 하게 된다.
			m_TotalAllocCount += ALLOC_COUNT;
		}

		// pAvailable은 mFreeList가 가리킬, 배열의 사용 가능한 현재 위치다!
		uint8_t* pAvailable = m_FreeList;
		// 그리고 mFreeList는 현재 위치에 담겨 있는 주소값(다음 위치겠지)으로 갱신
		m_FreeList = *reinterpret_cast<uint8_t**>( pAvailable );

		// 카운트 증가하고
		++m_CurrentUseCount;

		// 현재 위치 값을 리턴합시다.
		return pAvailable;
	}

	static void	operator delete( void* obj )
	{
		CRASH_ASSERT( m_CurrentUseCount > 0 );

		--m_CurrentUseCount;

		// mFreeList는 다음에 쓸, 사용 가능한 배열의 현재 위치
		// obj는 지금 지울 위치니까, 현재 지울 위치에 해당하는 값
		// 즉 *obj = mFreeList로, 현재 위치에 다음 위치 값을 저장한다
		*reinterpret_cast<uint8_t**>( obj ) = m_FreeList;

		// 그리고 현재 위치를, 사용 가능한 배열의 현재 위치로 갱신
		m_FreeList = static_cast<uint8_t*>( obj );
	}


private:
	static uint8_t*	m_FreeList;
	static int		m_TotalAllocCount; ///< for tracing
	static int		m_CurrentUseCount; ///< for tracing


};

// 정적 변수들에 대해서 초기화 필요
template <class TOBJECT, int ALLOC_COUNT>
uint8_t* ObjectPool<TOBJECT, ALLOC_COUNT>::m_FreeList = nullptr;

template <class TOBJECT, int ALLOC_COUNT>
int ObjectPool<TOBJECT, ALLOC_COUNT>::m_TotalAllocCount = 0;

template <class TOBJECT, int ALLOC_COUNT>
int ObjectPool<TOBJECT, ALLOC_COUNT>::m_CurrentUseCount = 0;


