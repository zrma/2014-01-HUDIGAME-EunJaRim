#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "GameRoom.h"
#include "MacroSet.h"
#include "GameMapManager.h"





std::hash_map<int, ClientSession*>	g_PidSessionTable;
RoomManager* g_RoomManager = nullptr;
RoomManager::RoomManager()
{
	m_GameMapManager = new GameMapManager();
	m_GameMapManager->Initialize();

	g_PidSessionTable.clear( );

	m_Lobby = new GameRoom( LOBBY_NUMBER, m_GameMapManager );
	
	m_RoomList.clear();
	m_RoomList.insert( RoomList::value_type( LOBBY_NUMBER, m_Lobby ) );

	// test code
	AddRoom();
	AddRoom();
	AddRoom();
	AddRoom();
	AddRoom();
	AddRoom();
	AddRoom();
	AddRoom();
	AddRoom();
	AddRoom();

}


RoomManager::~RoomManager()
{
	for ( auto it = m_RoomList.begin( ); it != m_RoomList.end( ); ++it )
	{
		GameRoom* room = it->second;
		delete room;
	}
	m_RoomList.clear();
}

int RoomManager::AddRoom()
{
	GameRoom* room = new GameRoom( ++m_RoomCount, m_GameMapManager );

	m_RoomList.insert( RoomList::value_type( m_RoomCount, room ) );

	Log( "ROOM [%d] CREATED! \n", room->GetRoomNumber() );

	return room->GetRoomNumber( );
}

bool RoomManager::EnterRoom( int roomNumber, int pid )
{
	Log( "EnterRoom [player:%d]->[%d] \n", pid, roomNumber );
	RoomList::iterator roomIter = m_RoomList.find( roomNumber );
	if ( roomIter == m_RoomList.end( ) )
	{
		// 다른 방을 찾아야함
		if ( QuickJoin( pid ) )
		{
			return true;
		}
		return false;
	}

	GameRoom* room = roomIter->second;
	if ( !room->IsGameRoomStart( ) && !room->IsGameRoomEnd() )
	{
		std::hash_map<int, ClientSession*>::iterator sessionIter = g_PidSessionTable.find( pid );
		if ( sessionIter != g_PidSessionTable.end() )
		{
			ClientSession* mover = sessionIter->second;
			if ( m_Lobby->LeaveGameRoom( mover ) )
			{
				room->EnterGameRoom( mover );
				return true;
			}
		}
	}
	else
	{
		// 다른 방을 찾아야함
		if ( QuickJoin( pid ) )
		{
			return true;
		}
	}

	return false;
}


bool RoomManager::QuickJoin( int pid )
{
	std::hash_map<int, ClientSession*>::iterator sessionIter = g_PidSessionTable.find( pid );
	if ( sessionIter != g_PidSessionTable.end() )
	{
		ClientSession* mover = sessionIter->second;
		for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
		{
			GameRoom* room = it->second;
			if ( LOBBY_NUMBER != room->GetRoomNumber() )
			{
				if ( !room->IsGameRoomStart( ) && !room->IsGameRoomEnd( ) )
				{
					if ( m_Lobby->LeaveGameRoom( mover ) )
					{
						room->EnterGameRoom( mover );
						return true;
					}
				}
			}
		}
	}

	// AddRoom, QuickJoin recursive? ClientSession careful
	return false;
}

bool RoomManager::LeaveRoom( int roomNumber, int pid )
{
	Log( "LeaveRoom [player:%d]->[%d] \n", pid, roomNumber );
	RoomList::iterator roomIter = m_RoomList.find( roomNumber );
	if ( roomIter == m_RoomList.end() )
	{
		return false;
	}
	else
	{
		std::hash_map<int, ClientSession*>::iterator sessionIter = g_PidSessionTable.find( pid );
		if ( sessionIter != g_PidSessionTable.end() )
		{
			ClientSession* mover = sessionIter->second;
			if ( roomIter->second->LeaveGameRoom( mover ) )
			{
				m_Lobby->EnterGameRoom( mover );
				return true;
			}
		}
	}

	return false;
}


bool RoomManager::DeleteRoom( int roomNumber )
{
	Log( "DeleteRoom [%d] \n", roomNumber );
	RoomList::iterator roomIter = m_RoomList.find( roomNumber );
	if ( roomIter == m_RoomList.end() )
	{
		return false;
	}
	else
	{
		GameRoom* toBeDelete = roomIter->second;
		if ( 0 == toBeDelete->GetClientSize() )
		{
			delete toBeDelete;
			m_RoomList.erase( roomNumber );
			PrintClientList();
			return true;
		}

		return false;
	}
}


bool RoomManager::CheckRoom( int roomNumber )
{
	if ( m_RoomList.find( roomNumber ) == m_RoomList.end() )
	{
		return false;
	}
	return true;
}


ClientSession* RoomManager::CreateClient( SOCKET sock )
{
	return m_Lobby->CreateClient( sock );
}

void RoomManager::FlushClientSend()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		GameRoom* room = it->second;
		room->FlushClientSend( );
	}
}

void RoomManager::OnPeriodWork()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		GameRoom* room = it->second;
		room->OnPeriodWork( );
	}

	ULONGLONG currTick = GetTickCount64();
	if ( currTick - m_LastGCRoomTick >= 3000 )
	{
		RoomList safetyRoomList = m_RoomList;
		for ( auto& it : safetyRoomList )
		{
			GameRoom* room = it.second;
			if ( room->IsGameRoomEnd() && !room->IsGameRoomStart() )
			{
				DeleteRoom( room->GetRoomNumber( ) );
				AddRoom(); // 일정수의 방 유지
			}
			
		}
		//PrintClientList();
		m_LastGCRoomTick = currTick;
	}
}

void RoomManager::PrintClientList()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		GameRoom* room = it->second;
		Log( "-ROOM %d ClientList- \n", room->GetRoomNumber() );
		room->PrintClientList();
	}
}
