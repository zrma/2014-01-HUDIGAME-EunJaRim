#pragma once


class CircularBuffer
{
public:

	// mBRegionPointer(nullptr), mARegionSize(0), mBRegionSize(0) 로 초기화
	//
	// B영역 포인터는 nullptr로 B영역은 아직 설정(사용) 되지 않음
	// A영역, B영역 모두 사용하지 않았으므로 사이즈는 0으로 초기화
	CircularBuffer( size_t capacity ): m_BRegionPointer( nullptr ), m_ARegionSize( 0 ), m_BRegionSize( 0 )
	{
		// 문자열 배열을 BUFSIZE (1024 * 10) 크기 만큼 할당하고 시작 주소 mBuffer 에 저장
		m_Buffer = new char[capacity];

		// 끝 주소값(= 시작 주소값 + 배열 사이즈) mBufferEnd 에 저장
		m_BufferEnd = m_Buffer + capacity;

		// 일단 A영역 포인터를 버퍼의 시작 주소값에 넣는다.
		m_ARegionPointer = m_Buffer;
	}

	~CircularBuffer()
	{
		delete[] m_Buffer;
	}

	// 데이터 가져오기
	bool Peek( OUT char* destbuf, size_t bytes ) const;

	// 읽기(읽고 난 데이터 삭제)
	bool Read( OUT char* destbuf, size_t bytes );

	// 쓰기
	bool Write( const char* data, size_t bytes );


	// 버퍼의 첫부분 len만큼 날리기
	void Remove( size_t len );

	size_t GetFreeSpaceSize()
	{
		// B영역에 데이터 들어있으면 B~A 사이에 가용 가능한 공간을 리턴
		if ( m_BRegionPointer != nullptr )
		{
			return GetBFreeSpace( );
		}
		else
		{
			/// A 버퍼보다 더 많이 존재하면, B 버퍼로 스위치
			// 앞쪽 공간이 크면 앞에 B버퍼 설정하고, B버퍼를 쓸 수 있는 크기를 리턴하자
			if ( GetAFreeSpace() < GetSpaceBeforeA() )
			{
				AllocateB();
				return GetBFreeSpace();
			}
			// 아니면 A버퍼 뒤쪽 남은 공간(쓸 수 있는) 크기를 리턴하자
			else
			{
				return GetAFreeSpace( );
			}

		}
	}

	// A영역과 B영역 사용한 크기 총합
	size_t GetStoredSize() const
	{
		return m_ARegionSize + m_BRegionSize;
	}

	size_t GetContiguiousBytes() const
	{
		// A영역에 데이터가 들어있으면 A영역 사이즈를
		if ( m_ARegionSize > 0 )
		{
			return m_ARegionSize;
		}
		// A영역이 비어 있으면 B영역 사이즈를 리턴
		else
		{
			return m_BRegionSize;
		}
	}

	/// 쓰기가 가능한 위치 (버퍼의 끝부분) 반환
	void* GetBuffer() const
	{
		// 현재 B영역을 쓰고 있을 때
		if ( m_BRegionPointer != nullptr )
		{
			return m_BRegionPointer + m_BRegionSize;
		}
		// 현재 A영역을 쓰고 있을 때
		else
		{
			return m_ARegionPointer + m_ARegionSize;
		}
			
	}




	/// 커밋(aka. IncrementWritten)
	// 버퍼 영역 사이즈 증가 (쓰기 발생)
	void Commit( size_t len )
	{
		// B영역을 쓰고 있을 때
		if ( m_BRegionPointer != nullptr )
		{
			m_BRegionSize += len;
		}
		// A영역을 쓰고 있을 때
		else
		{
			m_ARegionSize += len;
		}
			
	}

	// 버퍼의 첫부분 리턴
	void* GetBufferStart() const
	{
		// A영역에 데이터가 들어있으면 일단 A영역 시작 주소를 리턴한다
		if ( m_ARegionSize > 0 )
		{
			return m_ARegionPointer;
		}
		// 아니면 B영역 시작 주소를 리턴한다.
		else
		{
			return m_BRegionPointer;
		}
			
	}


private:

	// B영역 시작 주소를 mBuffer(버퍼 시작 주소)로 설정
	void AllocateB()
	{
		m_BRegionPointer = m_Buffer;
	}

	// A영역 끝부터 (-> 방향으로) 버퍼 마지막 위치까지 남은 공간 크기
	size_t GetAFreeSpace() const
	{
		return ( m_BufferEnd - m_ARegionPointer - m_ARegionSize );
	}

	// A영역 시작부터 (<- 방향으로) 버퍼 시작 위치까지 남은 공간 크기
	size_t GetSpaceBeforeA() const
	{
		return ( m_ARegionPointer - m_Buffer );
	}

	// B영역 시작부터 (-> 방향으로) A영역 시작 위치까지 남은 공간 크기
	size_t GetBFreeSpace() const
	{
		// B영역을 아직 사용하지 않았다면 0
		if ( m_BRegionPointer == nullptr )
		{
			return 0;
		}

		return ( m_ARegionPointer - m_BRegionPointer - m_BRegionSize );
	}

private:

	char*	m_Buffer;
	char*	m_BufferEnd;

	char*	m_ARegionPointer;
	size_t	m_ARegionSize;

	char*	m_BRegionPointer;
	size_t	m_BRegionSize;

};
