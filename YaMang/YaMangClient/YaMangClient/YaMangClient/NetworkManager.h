#pragma once
#include "MacroSet.h"
#include "CircularBuffer.h"
#include "PacketType.h"

class NetworkManager:public Singleton<NetworkManager>
{
public:
	NetworkManager();
	~NetworkManager();

	bool	Init();
	void	Destroy();
	void	ProcessPacket();
	bool	Connect();
	bool	HandleMessage( WPARAM wParam, LPARAM lParam );
	
	//////////////////////////////////////////////////////////////////////////
	// 패킷 핸들러
	//////////////////////////////////////////////////////////////////////////
	void	HandleLoginResult( LoginResult& inPacket );
	void	HandleGameOverResult( GameOverResult& inPacket );
	void	HandleChatResult( ChatBroadcastResult& inPacket );
	void	HandleGenerateCorpsResult( GenerateCorpsResult& inPacket );
	void	HandleChangeCorpsFormationResult( ChangeCorpsFormationResult& inPacket );
	void	HandleMoveCorpsResult( MoveCorpsResult& inPacket );
	void	HandleStopCorpsResult( StopCorpsResult& inPacket );
	void	HandleRefreshUIResult( RefreshUIResult& inPacket );


	//////////////////////////////////////////////////////////////////////////
	// 리퀘스트 패킷 보내기
	//////////////////////////////////////////////////////////////////////////
	void	RequestChat( ChatBroadcastRequest& outPacket );

private:
	SOCKET			m_Socket = NULL;
	CircularBuffer	m_RecvBuffer;
	CircularBuffer	m_SendBuffer;
	int				m_MyPlayerId;
};

typedef void( *HandlerFunc )( PacketHeader& pktBase );
extern HandlerFunc HandlerTable[];