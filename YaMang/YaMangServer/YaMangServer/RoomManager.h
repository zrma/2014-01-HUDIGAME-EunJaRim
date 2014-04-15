#pragma once

class ClientSession;
class ClientManager;

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	void	AddRoom();		// 방 생성
	void	DeleteRoom();	// 방 삭제
	void	ChangeRoom();	// 클라이언트를 다른 방으로 이동 (클라 매니저 아님)

	ClientSession*	CreateClient( SOCKET sock );
	void			FlushClientSend( );
	void			OnPeriodWork( );


	//////////////////////////////////////////////////////////////////////////
	// 임시 구현
	// 나중에 지울 것!
	ClientManager*	GetFirstClientManager()
	{
		if ( m_RoomList.find( 0 ) != m_RoomList.end() )
		{
			return m_RoomList[0];
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////

private:
	typedef std::map<int, ClientManager*> RoomList;
	RoomList	m_RoomList;
};

extern RoomManager* g_RoomManager;

