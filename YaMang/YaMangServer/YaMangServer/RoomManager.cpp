#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "ClientManager.h"



std::hash_map<int, ClientSession*>	g_PidSessionTable;
RoomManager* g_RoomManager = nullptr;
RoomManager::RoomManager()
{
	g_PidSessionTable.clear( );
	
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
	m_RoomList.clear();
}

int RoomManager::AddRoom()
{
	Room room;
	room.roomNumber = ++m_RoomCount;
	room.clientManager = new ClientManager( );

	// Test room start;
	room.clientManager->GameStart( );

	m_RoomList.push_back( room );

	printf_s( "ROOM [%d] CREATED! \n", room.roomNumber );

	return room.roomNumber;
}

bool RoomManager::ChangeRoom( int roomNumberFrom, int roomNumberTo, int pid )
{
	ClientManager* roomFrom = nullptr;
	ClientManager* roomTo = nullptr;
	ClientSession* sessionMover = g_PidSessionTable.find( pid )->second;

	///# 리스트로 하니까 이렇게 복잡해지는거다.

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

	if ( roomFrom->DeleteClient( sessionMover ) )
	{
		roomTo->InputClient( sessionMover );
		return true;
	}
	
	
	return false;
	
}

bool RoomManager::DeleteRoom( int roomNumber )
{
	///# QUIZ: 리스트는 순회중에 자신의 원소를 하나 지우면 안전한가?
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		if ( roomNumber == it->roomNumber )
		{

			ClientManager* toBeDelete = it->clientManager;
			if ( 0 == toBeDelete->GetClientSize( ) )
			{
				delete toBeDelete;
				m_RoomList.erase( it );
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

void RoomManager::PrintClientList()
{
	for ( auto it = m_RoomList.begin(); it != m_RoomList.end(); ++it )
	{
		ClientManager* room = it->clientManager;
		printf_s( "-ROOM %d ClientList- \n", it->roomNumber );
		room->PrintClientList();
	}
}
