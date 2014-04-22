#pragma once

#include <WinSock2.h>

class ClientSession;

struct PacketHeader;
struct DatabaseJobContext;


struct Tile
{
	///# 초기화 반드시 수행. 그리고 서버 코드에 RGB 있으면 이상하다.. 의미 위주로 네이밍 변경이나 주석 확실하게..
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

class ClientManager
{
public:
	ClientManager(): m_LastGCTick( 0 ), m_LastClientWorkTick( 0 )
	{
	}

	void				GameStart();


	ClientSession*		CreateClient( SOCKET sock );

	bool				DeleteClient( ClientSession* client );
	void				InputClient( ClientSession* client );

	int					GetClientSize() { return static_cast<int>(m_ClientList.size()); }

	void				BroadcastPacket( ClientSession* from, PacketHeader* pkt );
	bool				DirectPacket( int pid, PacketHeader* pkt );

	void				OnPeriodWork();


	void				ReadMapFile( const char* filename );


	/// DB에 플레이어 정보를 생성하거나 삭제하는 함수
	void				DBCreatePlayer( int pid, double x, double y, double z, const char* name, const char* comment );
	void				DBDeletePlayer( int pid );

	void				FlushClientSend();

	void				PrintClientList( ); // 테스트용 함수




private:
	void				CreatePlayerDone( DatabaseJobContext* dbJob );
	void				DeletePlayerDone( DatabaseJobContext* dbJob );

private:
	void				CollectGarbageSessions();
	void				ClientPeriodWork();
	void				DispatchDatabaseJobResults();


private:
	typedef std::map<SOCKET, ClientSession*> ClientList;
	ClientList			m_ClientList;

	DWORD				m_LastGCTick;
	DWORD				m_LastClientWorkTick;

	///# 백터의 백터인데.. Map이라는 네이밍을 쓰지 말고 BattleMap이라던가.. 
	///# 그리고 자주 참조하는 맵이고 게임 도중 변경될 일이 없는 맵이면 백터로 들고 있을 이유가 없음.
	std::vector<std::vector<Tile>>	m_Map;

	bool				m_IsGameStart = false;
};