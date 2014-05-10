#pragma once

#include "Config.h"
#include "PacketType.h"
#include "ObjectPool.h"


/// 모든 DB관련 작업은 아래의 추상 클래스를 상속받아 사용해야 함
struct DatabaseJobContext
{
	DatabaseJobContext(): m_Success( false )
	{
	}

	DatabaseJobContext( SOCKET socketKey ): m_SockKey( socketKey ), m_Success( false )
	{
	}

	virtual ~DatabaseJobContext() {}

	// EasyServer.cpp 의 DB 핸들링 스레드에서 무한루프 돌면서 OnExecute() 실행
	// 순수가상 함수 OnExecute()를 상속 받은 각각의 자식 Context가 실제 작업 수행
	virtual bool OnExecute() = 0;

	void SetSuccess( bool success ) { m_Success = success; }

	SOCKET	m_SockKey;
	bool	m_Success;
};


/// player load 작업
struct LoadPlayerDataContext: public DatabaseJobContext, public ObjectPool<LoadPlayerDataContext>
{
	LoadPlayerDataContext( SOCKET socketKey, int playerId ): DatabaseJobContext( socketKey )
	, m_PlayerId( playerId ), m_PosX( 0 ), m_PosY( 0 ), m_PosZ( 0 )
	{
		memset( m_PlayerName, 0, sizeof( m_PlayerName ) );
	}


	virtual bool OnExecute();


	int		m_PlayerId;

	double	m_PosX;
	double	m_PosY;
	double	m_PosZ;
	char	m_PlayerName[MAX_NAME_LEN];

};


/// Player 생성 작업
struct CreatePlayerDataContext: public DatabaseJobContext, public ObjectPool<CreatePlayerDataContext>
{
	CreatePlayerDataContext()
	: m_PlayerId( -1 ), m_PosX( 0 ), m_PosY( 0 ), m_PosZ( 0 )
	{
		memset( m_PlayerName, 0, sizeof( m_PlayerName ) );
		memset( m_Comment, 0, sizeof( m_Comment ) );
	}

	virtual bool OnExecute();

	int		m_PlayerId;

	double	m_PosX;
	double	m_PosY;
	double	m_PosZ;
	char	m_PlayerName[MAX_NAME_LEN];
	char	m_Comment[MAX_COMMENT_LEN];
};



/// Player 삭제 작업
struct DeletePlayerDataContext : public DatabaseJobContext, public ObjectPool<DeletePlayerDataContext>
{
	DeletePlayerDataContext(int playerId) : m_PlayerId(playerId)
	{}

	virtual bool OnExecute() ;

	int		m_PlayerId ;

} ;

/// Player 업데이트 작업
struct UpdatePlayerDataContext: public DatabaseJobContext, public ObjectPool<UpdatePlayerDataContext>
{
	UpdatePlayerDataContext( SOCKET socketKey, int playerId ): DatabaseJobContext( socketKey )
	, m_PlayerId( playerId ), m_PosX( 0 ), m_PosY( 0 ), m_PosZ( 0 )
	{
		memset( m_Comment, 0, sizeof( m_Comment ) );
	}

	virtual bool OnExecute();

	int		m_PlayerId;

	double	m_PosX;
	double	m_PosY;
	double	m_PosZ;
	char	m_Comment[MAX_COMMENT_LEN];
};
