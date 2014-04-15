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

	ClientSession* CreateClient( SOCKET sock );
	void FlushClientSend( );
	void OnPeriodWork( );


private:

	std::list<Room>				m_RoomList;
	ClientManager*				m_Lobby;
	int							m_RoomCount = 0;
};
extern std::hash_map<int, SOCKET>	g_PidSocketTable;
extern RoomManager*					g_RoomManager;

