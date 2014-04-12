#pragma once

template<typename TElem, int QSize>
class ProducerConsumerQueue
{
public:
	ProducerConsumerQueue(): m_OccupiedSize( 0 ), m_QueueOffset( 0 )
	{
		memset( m_QueueArray, 0, sizeof( m_QueueArray ) );
		InitializeConditionVariable( &m_NotFull );
		InitializeConditionVariable( &m_NotEmpty );
		InitializeSRWLock( &m_SRWLock );
	}

	~ProducerConsumerQueue() {}

	bool Produce( const TElem& item, bool waitUntilConsume = true )
	{
		/// 큐에 넣을 공간 생길때까지 잔다.
		AcquireSRWLockExclusive( &m_SRWLock );

		while ( m_OccupiedSize == QSize )
		{
			if ( waitUntilConsume )
			{
				/// 큐에 넣을 공간 생길때까지 잔다.
				SleepConditionVariableSRW( &m_NotFull, &m_SRWLock, INFINITE, 0 );
			}
			else
			{
				ReleaseSRWLockExclusive( &m_SRWLock );
				return false;
			}
		}


		m_QueueArray[( m_QueueOffset + m_OccupiedSize ) % QSize] = item;
		++m_OccupiedSize;

		ReleaseSRWLockExclusive( &m_SRWLock );

		WakeConditionVariable( &m_NotEmpty );

		return true;
	}


	bool Consume( TElem& item, bool waitUntilProduce = true )
	{
		/// 큐에 아이템 들어올때까지 잔다
		AcquireSRWLockExclusive( &m_SRWLock );

		while ( m_OccupiedSize == 0 )
		{
			if ( waitUntilProduce )
			{
				/// 큐에 아이템 들어올때까지 잔다
				SleepConditionVariableSRW( &m_NotEmpty, &m_SRWLock, INFINITE, 0 );
			}
			else
			{
				ReleaseSRWLockExclusive( &m_SRWLock );
				return false;
			}

		}

		item = m_QueueArray[m_QueueOffset];
		--m_OccupiedSize;

		if ( ++m_QueueOffset == QSize )
		{
			m_QueueOffset = 0;
		}

		ReleaseSRWLockExclusive( &m_SRWLock );

		WakeConditionVariable( &m_NotFull );

		return true;
	}

private:

	TElem m_QueueArray[QSize];

	uint32_t m_OccupiedSize;
	uint32_t m_QueueOffset;

	CONDITION_VARIABLE m_NotFull;
	CONDITION_VARIABLE m_NotEmpty;

	SRWLOCK m_SRWLock;
};


