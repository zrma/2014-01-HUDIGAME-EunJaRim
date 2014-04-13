#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "ClientManager.h"


RoomManager* g_RoomManager = nullptr;

RoomManager::RoomManager()
{
	ClientManager* waitingRoom = new ClientManager();
	//m_RoomList.push_back( RoomList::value_type( 0, waitingRoom ) ); // 0은 대기방을 뜻함
	m_RoomList.insert( RoomList::value_type( 0, waitingRoom ) ); // 0은 대기방을 뜻함
}


RoomManager::~RoomManager()
{
	for ( auto& it : m_RoomList )
	{
		ClientManager* room = it.second;
		delete room;
	}
}

ClientSession* RoomManager::CreateClient( SOCKET sock )
{
	// 순회를 할 필요 없으나 나중에 map에서 vector로 바꿀예정
	for ( auto& it : m_RoomList )
	{
		// 대기방에 생성
		int roomNumber = it.first;
		if ( 0 == roomNumber )
		{
			ClientManager* room = it.second;
			return room->CreateClient( sock );
		}
	}

	CRASH_ASSERT( false );
	return nullptr;
}

void RoomManager::FlushClientSend()
{
	for ( auto& it : m_RoomList )
	{
		ClientManager* room = it.second;
		room->FlushClientSend();
	}
}

void RoomManager::OnPeriodWork()
{
// 	for ( auto& it = m_RoomList.begin( ); it != m_RoomList.end( ); ++it )
// 	{
// 		ClientManager* room = it.second;
// 		room->OnPeriodWork();
// 	}
	for ( auto& it : m_RoomList )
	{
		ClientManager* room = it.second;
		room->OnPeriodWork();
	}
}
