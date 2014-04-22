#pragma once

class ClientSession;
class ClientManager;

struct Room
{
	///# 코딩 컨벤션 통일... 멤버 변수 앞에는 m_했으면 끝까지 m_ ....

	///# 룸 넘버 -1이라고 쓰기 보다... 아래처럼 쓰기.
	enum 
	{
		INVALID_ROOM = -1
	};

	Room(): roomNumber( -1 ), clientManager( nullptr ) {}

	int roomNumber;
	ClientManager* clientManager;
};

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	int 	AddRoom(); // 방 생성
	///# 아래 함수가 왜 필요하지? Lobby - Room 이동만 있는거 아닌가? 
	/// Room -> Room이동이 있나? 만일 없다면 아래 함수 필요가 없음. 그냥 EnterGame(Lobby to Room) / LeaveGame (Room to Lobby) 있으면 될 것 같은데?
	bool	ChangeRoom( int roomNumberFrom, int roomNumberTo, int pid ); // 클라이언트를 다른 방으로 이동 (클라 매니저 아님)
	bool	DeleteRoom( int roomNumber ); // 방 삭제

	ClientSession*	CreateClient( SOCKET sock );
	void			FlushClientSend( );
	void			OnPeriodWork( );



	void			PrintClientList(); // 테스트용 함수


public:
	ClientManager*					m_Lobby;

private:

	///# 리스트로 한 이유는? 
	std::list<Room>					m_RoomList;
	int								m_RoomCount = 0;



};
extern std::hash_map<int, ClientSession*>	g_PidSessionTable;
extern RoomManager*							g_RoomManager;

