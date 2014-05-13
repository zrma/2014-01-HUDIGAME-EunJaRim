#include "stdafx.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "GameRoom.h"
#include "MacroSet.h"



const int LOBBY_NUMBER = 0;

std::hash_map<int, ClientSession*>	g_PidSessionTable; ///# STL 객체는 전역으로 놓지 말 것. 다른 클래스 안에 넣거나 전용 클래스를 새로 만들거나.. 
///# 꼭 전역으로 놓아겠으면, 반드시 포인터의 형태로.

RoomManager* g_RoomManager = nullptr;
RoomManager::RoomManager()
{
	g_PidSessionTable.clear( );

	m_Lobby = new GameRoom( LOBBY_NUMBER );
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

	///# 로비도 delete 해야 되는거 아닌가? 물론, 프로그램 종료시까지 살아 있으니까 안해도 되긴하지만, 표시는 해줄것.
}

int RoomManager::AddRoom()
{
	GameRoom* room = new GameRoom( ++m_RoomCount );

	// Test room start;
	room->GameStart( ); ///# 이 코드가 아래 룸리스트 다음에 와야 되는거 아닌가?
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
			ClientSession* mover = g_PidSessionTable.find( pid )->second;  ///# 헐.. 왜 find를 두번? 
			if ( m_Lobby->DeleteClient( mover ) )
			{
				m_RoomList.find( roomNumber )->second->InputClient( mover ); ///# roomNumber에 해당하는 이터래이터가 없으면?
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
			ClientSession* mover = g_PidSessionTable.find( pid )->second; ///# find 한번만 해도 되자너.
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
