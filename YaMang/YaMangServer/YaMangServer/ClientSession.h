#pragma once

#include "Config.h"
#include "PacketType.h"
#include "CircularBuffer.h"
#include "ObjectPool.h"

#define BUFSIZE	(1024*10)

class ClientSession;
class ClientManager;
struct DatabaseJobContext;

struct OverlappedIO: public OVERLAPPED
{
	OverlappedIO(): m_Object( nullptr )
	{
	}

	ClientSession* m_Object;
};

class ClientSession: public ObjectPool<ClientSession>
{
public:
	ClientSession( SOCKET sock )
		: m_Connected( false ), m_Logon( false ), m_Socket( sock ), m_PlayerId( -1 ), m_SendBuffer( BUFSIZE ), m_RecvBuffer( BUFSIZE ), m_OverlappedRequested( 0 )
		, m_PosX( 0 ), m_PosY( 0 ), m_PosZ( 0 ), m_DbUpdateCount( 0 )
	{
		memset( &m_ClientAddr, 0, sizeof( SOCKADDR_IN ) );
		memset( m_PlayerName, 0, sizeof( m_PlayerName ) );
	}
	~ClientSession() {}

	void	OnRead( size_t len );
	void	OnWriteComplete( size_t len );

	bool	OnConnect( SOCKADDR_IN* addr );

	bool	PostRecv();

	bool	SendRequest( PacketHeader* pkt );
	bool	Broadcast( PacketHeader* pkt );
	bool	DirectSend( PacketHeader* pkt );

	void	Disconnect();

	bool	IsConnected() const { return m_Connected; }

	void	DatabaseJobDone( DatabaseJobContext* result );


	/// 현재 Send/Recv 요청 중인 상태인지 검사하기 위함
	void	IncOverlappedRequest() { ++m_OverlappedRequested; }
	void	DecOverlappedRequest() { --m_OverlappedRequested; }
	bool	DoingOverlappedOperation() const { return m_OverlappedRequested > 0; }

	void	SetClientManager( ClientManager* clientManager ) { m_ClientManager = clientManager; }

	//////////////////////////////////////////////////////////////////////////
	// 패킷 핸들러
	//////////////////////////////////////////////////////////////////////////
	void	HandleLoginRequest( LoginRequest& inPacket );
	void	HandleChatRequest( ChatBroadcastRequest& inPacket );
	void	HandleGameOverRequest( GameOverRequest& inPacket );
	void	HandleRoomCreateRequest( RoomCreateRequest& inPacket );
	void	HandleRoomChangeRequest( RoomChangeRequest& inPacket );


private:

	bool	SendFlush(); ///< Send요청 중인것들 모아서 보냄
	void	OnTick();

	void	LoginDone( int pid, double x, double y, double z, const char* name );
	void	UpdateDone();


private:
	double			m_PosX;
	double			m_PosY;
	double			m_PosZ;
	char			m_PlayerName[MAX_NAME_LEN];

private:
	bool			m_Connected;
	bool			m_Logon;
	SOCKET			m_Socket;

	int				m_PlayerId;
	SOCKADDR_IN		m_ClientAddr;

	CircularBuffer	m_SendBuffer;
	CircularBuffer	m_RecvBuffer;

	OverlappedIO	m_OverlappedSend;
	OverlappedIO	m_OverlappedRecv;
	int				m_OverlappedRequested;

	int				m_DbUpdateCount; ///< DB에 주기적으로 업데이트 하기 위한 변수

	ClientManager*	m_ClientManager;

	friend class ClientManager;
};

typedef void( *HandlerFunc )( ClientSession* session, PacketHeader& pktBase );
extern HandlerFunc HandlerTable[];

void CALLBACK RecvCompletion( DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags );
void CALLBACK SendCompletion( DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags );
