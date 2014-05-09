#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "ClientManager.h"



const int LOBBY_NUMBER = 0;

std::hash_map<int, ClientSession*>	g_PidSessionTable;
RoomManager* g_RoomManager = nullptr;
RoomManager::RoomManager()
{
	g_PidSessionTable.clear( );

	m_Lobby = new ClientManager( LOBBY_NUMBER );

	m_RoomList.clear();
	m_RoomList.insert( RoomList::value_type( LOBBY_NUMBER, m_Lobby ) );
}


RoomManager::~RoomManager()
{
	for ( auto it = m_RoomList.begin( ); it != m_RoomList.end( ); ++it )
	{
		ClientManager* room = it->second;
		delete room;
	}
	m_RoomList.clear();
}

int RoomManager::AddRoom()
{
	ClientManager* room = new ClientManager( ++m_RoomCount );

	// Test room start;
	room->GameStart( );
	m_RoomList.insert( RoomList::value_type( m_RoomCount, room ) );

	printf_s( "ROOM [%d] CREATED! \n", room->GetRoomNumber() );

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
		ClientManager* toBeDelete = m_RoomList.find( roomNumber )->second;
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
		ClientManager* room = it->second;
		room->FlushClientSend( );
	}
}

void RoomManager::OnPeriodWork()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		ClientManager* room = it->second;
		room->OnPeriodWork( );
	}
}

void RoomManager::PrintClientList()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		ClientManager* room = it->second;
		printf_s( "-ROOM %d ClientList- \n", room->GetRoomNumber() );
		room->PrintClientList();
	}
}
