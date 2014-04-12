#include "stdafx.h"
#include "CircularBuffer.h"
#include <assert.h>

// 정해진 크기만큼 읽어만 오기
// 패킷 헤더를 읽어올 때 사용
bool CircularBuffer::Peek( OUT char* destbuf, size_t bytes ) const
{
	// 생성자에서 new 했어야 됐다.
	// nullptr이면 동적 메모리 할당에 문제가 있는 것
	assert( m_Buffer != nullptr );

	// bytes 만큼 읽기와 보기로 했는데 버퍼 총 영역 사이즈가 이것보다 작다.
	// 아직 패킷 헤더 크기만큼 데이터가 오지도 않은 것임
	if ( m_ARegionSize + m_BRegionSize < bytes )
	{
		return false;
	}

	size_t cnt = bytes;
	size_t aRead = 0;

	/// A, B 영역 둘다 데이터가 있는 경우는 A먼저 읽는다
	if ( m_ARegionSize > 0 )
	{
		// 읽으려는 크기(cnt)가 A영역에 담긴 데이터 양보다 크면 - case 1
		// 일단 읽는 양(aRead)을 A영역 사이즈로 설정
		// 아니라면 읽으려는 크기만큼 전체 읽기
		// case 1에서 나머지 데이터는 B 영역에 있을 것임
		aRead = ( cnt > m_ARegionSize ) ? m_ARegionSize : cnt;
		memcpy( destbuf, m_ARegionPointer, aRead );
		cnt -= aRead;
		// cnt > 0이라면, B영역에서 나머지 데이터를 읽어 와야 됨
	}

	/// 읽기 요구한 데이터가 더 있다면 B 영역에서 읽는다
	if ( cnt > 0 && m_BRegionSize > 0 )
	{
		// A 읽어오고 B를 읽은 건데
		// 읽을 데이터 양이 B에 담겨 있는 전체 데이터 양보다 크다면 이상한 거다.
		assert( cnt <= m_BRegionSize );

		/// 남은거 마저 다 읽기
		size_t bRead = cnt;

		memcpy( destbuf + aRead, m_BRegionPointer, bRead );
		cnt -= bRead;
	}

	// A버퍼 + B버퍼 데이터 싹 다 읽어와서 지정한 크기만큼 모두 읽었어야 하는데
	// cnt(읽을 양 남은 카운트 수) 가 0 이 아니면 잘못 된 거다.
	//
	// 저 위의 if( mARegionSize + mBRegionSize < bytes ) return false ; 구문에서 걸러졌어야 됨
	assert( cnt == 0 );

	return true;

}

bool CircularBuffer::Read( OUT char* destbuf, size_t bytes )
{
	assert( m_Buffer != nullptr );

	// 여기까지는 Peek() 와 동일한 방어코드
	if ( m_ARegionSize + m_BRegionSize < bytes )
	{
		return false;
	}


	size_t cnt = bytes;
	size_t aRead = 0;


	/// A, B 영역 둘다 데이터가 있는 경우는 A먼저 읽는다
	if ( m_ARegionSize > 0 )
	{
		aRead = ( cnt > m_ARegionSize ) ? m_ARegionSize : cnt;
		memcpy( destbuf, m_ARegionPointer, aRead );

		// Peek() 함수와 다르게 더 추가 된 부분
		// A버퍼 읽었으니 A버퍼 시작 포인터는 읽은 만큼(aRead) 증가시키고
		// A버퍼의 사이즈는 감소 시킨다.
		m_ARegionSize -= aRead;
		m_ARegionPointer += aRead;
		cnt -= aRead;
	}

	/// 읽기 요구한 데이터가 더 있다면 B 영역에서 읽는다
	if ( cnt > 0 && m_BRegionSize > 0 )
	{
		assert( cnt <= m_BRegionSize );

		/// 남은거 마저 다 읽기
		size_t bRead = cnt;

		memcpy( destbuf + aRead, m_BRegionPointer, bRead );
		m_BRegionSize -= bRead;
		m_BRegionPointer += bRead;

		// 마찬가지로 Peek() 함수와 다르게 더 추가 된 부분
		// 버퍼 안의 데이터들을 읽은 후에 제거시킴(실제로는 가리키는 포인터 이동)
		cnt -= bRead;
	}

	assert( cnt == 0 );

	/// A 버퍼가 비었다면 B버퍼를 맨 앞으로 당기고 A 버퍼로 지정 
	if ( m_ARegionSize == 0 )
	{
		// B버퍼에 데이터가 있을 때
		if ( m_BRegionSize > 0 )
		{
			// B버퍼가 맨 앞이 아니면
			if ( m_BRegionPointer != m_Buffer )
			{
				memmove( m_Buffer, m_BRegionPointer, m_BRegionSize );
			}

			// B버퍼를 A버퍼로 바꿈
			m_ARegionPointer = m_Buffer;
			m_ARegionSize = m_BRegionSize;

			// B버퍼는 초기화
			m_BRegionPointer = nullptr;
			m_BRegionSize = 0;

			// 이 상태가 되면 A버퍼에만 데이터가 남아 있는 상태임
		}
		else
		{
			/// B에 아무것도 없는 경우 그냥 A로 스위치
			// B버퍼에 아무 것도 없으므로 바로 초기화
			m_BRegionPointer = nullptr;
			m_BRegionSize = 0;

			// A버퍼가 이미 비어 있으므로 A버퍼도 초기화
			m_ARegionPointer = m_Buffer;
			m_ARegionSize = 0;

			// 이 상태가 되면 CircularBuffer() 초기 생성 상태임
		}
	}

	return true;
}




bool CircularBuffer::Write( const char* data, size_t bytes )
{
	assert( m_Buffer != nullptr );

	/// Read와 반대로 B가 있다면 B영역에 먼저 쓴다
	// 읽을 때 A부터 읽는다.
	// 쓸 때는 B부터 먼저 쓴다.
	// 시간적으로 A에 담겨 있는 데이터가 언제나 먼저 온 데이터
	if ( m_BRegionPointer != nullptr )
	{
		// B버퍼에 데이터가 담겨 있어서, B버퍼에 이어 쓰려고 했는데 공간이 없다
		if ( GetBFreeSpace() < bytes )
		{
			return false;
		}

		// B버퍼 쓰던 곳에 이어서 쓰기
		memcpy( m_BRegionPointer + m_BRegionSize, data, bytes );
		m_BRegionSize += bytes;

		return true;
	}

	// B에 데이터가 있으면 위의 if문에서 걸러진 후 어떻게든 return 하므로
	// 하단은 무조건 B가 비어 있는 상태임 = A에만 데이터가 있는 상태
	/// A영역보다 다른 영역의 용량이 더 클 경우 그 영역을 B로 설정하고 기록

	// A버퍼가 충분히 뒤쪽으로 밀려난 상태이므로
	// 앞쪽에 B버퍼를 새로 만들어서 거기에 데이터를 쓰기
	if ( GetAFreeSpace() < GetSpaceBeforeA() )
	{
		AllocateB();

		// B버퍼 끝부분(부터 쓸 수 있는 공간)에서 A버퍼 시작부분까지 사이의 공간보다
		// 써야 할 데이터 양이 더 많은 상황이므로 못 넣음
		if ( GetBFreeSpace() < bytes )
		{
			return false;
		}

		memcpy( m_BRegionPointer + m_BRegionSize, data, bytes );
		m_BRegionSize += bytes;

		return true;
	}
	/// A영역이 더 크면 당연히 A에 쓰기
	// A버퍼 뒤쪽에 쭉 이어서 쓰기
	else
	{
		if ( GetAFreeSpace() < bytes )
		{
			return false;
		}

		memcpy( m_ARegionPointer + m_ARegionSize, data, bytes );
		m_ARegionSize += bytes;

		return true;
	}
}



void CircularBuffer::Remove( size_t len )
{
	size_t cnt = len;

	/// Read와 마찬가지로 A가 있다면 A영역에서 먼저 삭제

	if ( m_ARegionSize > 0 )
	{
		size_t aRemove = ( cnt > m_ARegionSize ) ? m_ARegionSize : cnt;
		m_ARegionSize -= aRemove;
		m_ARegionPointer += aRemove;
		cnt -= aRemove;
	}

	// 제거할 용량이 더 남은경우 B에서 제거 
	if ( cnt > 0 && m_BRegionSize > 0 )
	{
		size_t bRemove = ( cnt > m_BRegionSize ) ? m_BRegionSize : cnt;
		m_BRegionSize -= bRemove;
		m_BRegionPointer += bRemove;
		cnt -= bRemove;
	}

	/// A영역이 비워지면 B를 A로 스위치 
	if ( m_ARegionSize == 0 )
	{
		if ( m_BRegionSize > 0 )
		{
			/// 앞으로 당겨 붙이기
			if ( m_BRegionPointer != m_Buffer )
			{
				memmove( m_Buffer, m_BRegionPointer, m_BRegionSize );
			}


			m_ARegionPointer = m_Buffer;
			m_ARegionSize = m_BRegionSize;
			m_BRegionPointer = nullptr;
			m_BRegionSize = 0;
		}
		else
		{
			m_BRegionPointer = nullptr;
			m_BRegionSize = 0;
			m_ARegionPointer = m_Buffer;
			m_ARegionSize = 0;
		}
	}
}


