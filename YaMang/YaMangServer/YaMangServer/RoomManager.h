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

	void	AddRoom(); // 방 생성
	bool	ChangeRoom( int roomNumberFrom, int roomNumberTo, int pid ); // 클라이언트를 다른 방으로 이동 (클라 매니저 아님)
	bool	DeleteRoom( int roomNumber ); // 방 삭제

	ClientSession*	CreateClient( SOCKET sock );
	void			FlushClientSend( );
	void			OnPeriodWork( );


	//////////////////////////////////////////////////////////////////////////
	// 임시 구현
	// 나중에 지울 것!
	ClientManager*	GetFirstClientManager()
	{
		if ( m_RoomList.size() > 0 )
		{
			return (m_RoomList.begin())->clientManager;
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

