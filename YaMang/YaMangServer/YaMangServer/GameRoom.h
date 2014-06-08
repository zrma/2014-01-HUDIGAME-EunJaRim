#pragma once

#include <WinSock2.h>
#include "EnumSet.h"
#include "GameMapManager.h"

class Corps;
class ClientSession;
class ActionScheduler;
class Action;

struct PositionInfo;
struct PacketHeader;
struct DatabaseJobContext;

class GameRoom
{
public:
	GameRoom( int roomNumber, GameMapManager* gameMapManager );
	~GameRoom();

	void				GameRoomStart();
	void				GameRoomGiveUp( );
	void				GameRoomLoose( int loserPlayerID );

	ClientSession*		CreateClient( SOCKET sock );

	void				EnterGameRoom( ClientSession* client );
	bool				LeaveGameRoom( ClientSession* client );


	bool				IsGameRoomStart() const { return m_GameRoomStart; }
	bool				IsGameRoomEnd() const { return m_GameRoomEnd; }

	int					GetClientSize() const { return static_cast<int>( m_ClientList.size() ); }
	int					GetRoomNumber() const { return m_RoomNumber; }

	void				BroadcastPacket( PacketHeader* pkt );

	bool				DirectPacket( int pid, PacketHeader* pkt );

	void				OnPeriodWork();


	




	const Corps*		GenerateCorps( int playerID, UnitType type, PositionInfo position );

	typedef std::map<int, Corps*> CorpsList;
	const CorpsList&	GetCorpsList() { return m_CorpsList; }

	void				AddActionToScheduler( Action* addedAction, ULONGLONG remainTime );


	/// DB에 플레이어 정보를 생성하거나 삭제하는 함수
	void				DBCreatePlayer( int pid, double x, double y, double z, const char* name, const char* comment );
	void				DBDeletePlayer( int pid );

	void				FlushClientSend();




	void				PrintClientList(); // 테스트용 함수

	Corps*				GetCorpsByCorpsID( int corpsID );
	int					GetGuardIndexByID( int corpsID );
	int					GetKingIndexByID( int corpsID );


	const PositionInfo&	GetGuardPositionInfo( int index ) const;
	const PositionInfo&	GetKingPositionInfo( int index ) const;

	const MapTileType&		GetMapTileType( int x, int z ) const { return m_GameMapManager->GetMapTileType( x, z ); }
	const MapObjectType&	GetMapObjectType( int x, int z ) const { return m_GameMapManager->GetMapObjectType( x, z ); }
	const float&	GetMapHeight( int x, int z ) const { return m_GameMapManager->GetMapHeight( x, z ); }

	void				TakeBase( int ownerPlayerID, int targetPlayerID, int ownerCorpsID, int targetGuardID );
	void				SyncOneCorp( int corpsID );




private:
	void				CreatePlayerDone( DatabaseJobContext* dbJob );
	void				DeletePlayerDone( DatabaseJobContext* dbJob );

private:
	void				CollectGarbageSessions();
	void				ClientPeriodWork();
	void				DispatchDatabaseJobResults();


private:
	int					m_RoomNumber = -1;
	GameMapManager*		m_GameMapManager = nullptr;

	typedef std::map<SOCKET, ClientSession*> ClientList;
	ClientList			m_ClientList;

	
	CorpsList			m_CorpsList;
	int					m_CorpsIDCount = 0;

	ULONGLONG			m_LastGCTick;
	ULONGLONG			m_LastClientWorkTick;





	std::vector<int>	m_KingIDList;
	std::vector<int>	m_GuardIDList;


	bool				m_GameRoomStart = false;
	bool				m_GameRoomEnd = false;
	ActionScheduler*	m_ActionScheduler = nullptr;
};