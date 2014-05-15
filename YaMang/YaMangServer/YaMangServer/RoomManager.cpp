#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "GameRoom.h"
#include "MacroSet.h"
#include "GameMapManager.h"



const int LOBBY_NUMBER = 0;

std::hash_map<int, ClientSession*>	g_PidSessionTable;
RoomManager* g_RoomManager = nullptr;
RoomManager::RoomManager()
{
	m_GameMapManager = new GameMapManager();
	m_GameMapManager->Initialize();

	g_PidSessionTable.clear( );

	m_Lobby = new GameRoom( LOBBY_NUMBER, m_GameMapManager );
	// Test room start;
	m_Lobby->GameStart( );

	m_RoomList.clear();
	m_RoomList.insert( RoomList::value_type( LOBBY_NUMBER, m_Lobby ) );
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

	// Test room start;
	room->GameStart( );
	m_RoomList.insert( RoomList::value_type( m_RoomCount, room ) );

	Log( "ROOM [%d] CREATED! \n", room->GetRoomNumber() );

	return room->GetRoomNumber( );
}

bool RoomManager::EnterRoom( int roomNumber, int pid )
{

	if ( m_RoomList.find( roomNumber ) == m_RoomList.end() )
	{
		return false;
	}
	else
	{
		if ( g_PidSessionTable.find( pid ) != g_PidSessionTable.end() )
		{
			ClientSession* mover = g_PidSessionTable.find( pid )->second;
			if ( m_Lobby->DeleteClient( mover ) )
			{
				m_RoomList.find( roomNumber )->second->InputClient( mover );
				return true;
			}
		}
	}

	return false;
}

bool RoomManager::LeaveRoom( int roomNumber, int pid )
{
	if ( m_RoomList.find( roomNumber ) == m_RoomList.end() )
	{
		return false;
	}
	else
	{
		if ( g_PidSessionTable.find( pid ) != g_PidSessionTable.end() )
		{
			ClientSession* mover = g_PidSessionTable.find( pid )->second;
			if ( m_RoomList.find( roomNumber )->second->DeleteClient( mover ) )
			{
				m_Lobby->InputClient( mover );
				return true;
			}
		}
	}

	return false;
}


bool RoomManager::DeleteRoom( int roomNumber )
{
	if ( m_RoomList.find( roomNumber ) == m_RoomList.end() )
	{
		return false;
	}
	else
	{
		GameRoom* toBeDelete = m_RoomList.find( roomNumber )->second;
		if ( 0 == toBeDelete->GetClientSize() )
		{
			delete toBeDelete;
			m_RoomList.erase( roomNumber );
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
