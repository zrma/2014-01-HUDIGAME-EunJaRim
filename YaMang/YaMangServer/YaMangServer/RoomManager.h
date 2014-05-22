#pragma once

class ClientSession;
class GameRoom;
class GameMapManager;

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	int 	AddRoom(); // 방 생성
	
	bool	EnterRoom( int roomNumber, int pid );
	bool	LeaveRoom( int roomNumber, int pid );

	bool	QuickJoin( int pid );

	bool	DeleteRoom( int roomNumber ); // 방 삭제

	bool	CheckRoom( int roomNumber);

	ClientSession*	CreateClient( SOCKET sock );
	void			FlushClientSend( );
	void			OnPeriodWork( );



	void			PrintClientList(); // 테스트용 함수


public:
	GameRoom*					m_Lobby;

private:
	typedef std::map<int, GameRoom*> RoomList;
	RoomList						m_RoomList;
	int								m_RoomCount = 0;
	GameMapManager*					m_GameMapManager = nullptr;

	ULONGLONG						m_LastGCRoomTick = 0;
};
extern std::hash_map<int, ClientSession*>	g_PidSessionTable;
extern RoomManager*							g_RoomManager;

