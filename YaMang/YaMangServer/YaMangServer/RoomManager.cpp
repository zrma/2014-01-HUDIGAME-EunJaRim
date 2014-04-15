#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "ClientManager.h"

std::hash_map<int, SOCKET>	g_PidSocketTable;
RoomManager* g_RoomManager = nullptr;
RoomManager::RoomManager()
{
	g_PidSocketTable.clear();

	m_Lobby = new ClientManager( );

	Room room;
	room.roomNumber = 0;
	room.clientManager = m_Lobby;

	m_RoomList.clear();
	m_RoomList.push_back( room ); // 0은 대기방을 뜻함
}


RoomManager::~RoomManager()
{
	for ( auto it = m_RoomList.begin( ); it != m_RoomList.end( ); ++it )
	{
		ClientManager* room = it->clientManager;
		delete room;
	}
}

void RoomManager::AddRoom()
{
	Room room;
	room.roomNumber = ++m_RoomCount;
	room.clientManager = new ClientManager( );
	m_RoomList.push_back( room );
}

bool RoomManager::ChangeRoom( int roomNumberFrom, int roomNumberTo, int pid )
{
	ClientManager* roomFrom = nullptr;
	ClientManager* roomTo = nullptr;
	SOCKET socketMover = g_PidSocketTable.find( pid )->second;

	int roomCount = 0;
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		if ( roomNumberFrom == it->roomNumber )
		{
			roomFrom = it->clientManager;
			++roomCount;
		}
		else if ( roomNumberTo == it->roomNumber )
		{
			roomTo = it->clientManager;
			++roomCount;
		}

		if ( roomCount == 2 )
		{
			break;
		}
	}


	if ( nullptr == roomFrom || nullptr == roomTo )
	{
		return false;
	}


	if ( roomFrom->DeleteClient( socketMover ) )
	{
		roomTo->CreateClient( socketMover );
		return true;
	}

	return false;
}

bool RoomManager::DeleteRoom( int roomNumber )
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		if ( roomNumber == it->roomNumber )
		{

			ClientManager* toBeDelete = it->clientManager;
			if ( 0 == toBeDelete->GetClientSize( ) )
			{
				delete toBeDelete;
				return true;
			}

			return false;

		}
	}

	return false;
}

ClientSession* RoomManager::CreateClient( SOCKET sock )
{
	return m_Lobby->CreateClient( sock );
}

void RoomManager::FlushClientSend()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		ClientManager* room = it->clientManager;
		room->FlushClientSend( );
	}
}

void RoomManager::OnPeriodWork()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		ClientManager* room = it->clientManager;
		room->OnPeriodWork( );
	}
}

