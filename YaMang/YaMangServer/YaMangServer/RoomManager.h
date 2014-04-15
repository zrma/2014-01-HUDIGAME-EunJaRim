#pragma once
#include <list>
#include <hash_map>

class ClientSession;
class ClientManager;

struct Room
{
	Room(): roomNumber( -1 ), clientManager( nullptr ) {}

	int roomNumber;
	ClientManager* clientManager;
};


class RoomManager
{
public:
	RoomManager();
	~RoomManager();

<<<<<<< HEAD
	void	AddRoom();		// 방 생성
	void	DeleteRoom();	// 방 삭제
	void	ChangeRoom();	// 클라이언트를 다른 방으로 이동 (클라 매니저 아님)
=======
	void	AddRoom(); // 방 생성
	bool	ChangeRoom( int roomNumberFrom, int roomNumberTo, int pid ); // 클라이언트를 다른 방으로 이동 (클라 매니저 아님)
	bool	DeleteRoom( int roomNumber ); // 방 삭제
>>>>>>> f5e6817da0d90ae350ee46022773a6a1adee0659

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

	std::list<Room>				m_RoomList;
	ClientManager*				m_Lobby;
	int							m_RoomCount = 0;
};
extern std::hash_map<int, SOCKET>	g_PidSocketTable;
extern RoomManager*					g_RoomManager;

