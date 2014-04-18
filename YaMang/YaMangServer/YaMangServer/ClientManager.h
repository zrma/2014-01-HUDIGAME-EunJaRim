#pragma once

#include <map>
#include <WinSock2.h>

class ClientSession;

struct PacketHeader;
struct DatabaseJobContext;
struct Tile
{
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
	ClientSession*		DeleteClient( SOCKET sock );
	void				InputClient( ClientSession* client );

	int					GetClientSize() { return static_cast<int>(m_ClientList.size()); }

	void				BroadcastPacket( ClientSession* from, PacketHeader* pkt );
	void				DirectPacket( int pid, PacketHeader* pkt );

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


	std::vector<std::vector<Tile>>	m_Map;

	bool				m_IsGameStart = false;
};