#pragma once
#include "EnumSet.h"
#include "SharedDefine.h"

#define MAX_CHAT_LEN	1024

#define MAX_NAME_LEN	30
#define MAX_COMMENT_LEN	40

enum PacketTypes
{
	PKT_NONE = 0,

	PKT_CS_LOGIN = 1,
	PKT_SC_LOGIN = 2,

	PKT_CS_CHAT = 3,
	PKT_SC_CHAT = 4,

	PKT_CS_GAMEOVER = 5,
	PKT_SC_GAMEOVER = 6,

	PKT_CS_ROOM_CREATE = 7,
	PKT_SC_ROOM_CREATE = 8,

	PKT_CS_ROOM_CHANGE = 9,
	PKT_SC_ROOM_CHANGE = 10,

	PKT_CS_GENERATE_CORPS = 100,
	PKT_SC_GENERATE_CORPS = 101,

	PKT_CS_MOVE_CORPS = 102,
	PKT_SC_MOVE_CORPS = 103,

	PKT_CS_CORPS_CHANGE_FORMATION = 200,
	PKT_SC_CORPS_CHANGE_FORMATION = 201,

	PKT_MAX = 1024
};

#pragma pack(push, 1)

struct PacketHeader
{
	PacketHeader(): m_Size( 0 ), m_Type( PKT_NONE ) {}
	short m_Size;
	short m_Type;
};



struct LoginRequest: public PacketHeader
{
	LoginRequest()
	{
		m_Size = sizeof( LoginRequest );
		m_Type = PKT_CS_LOGIN;
		m_PlayerId = -1;
	}

	int	m_PlayerId;
};

struct LoginResult: public PacketHeader
{
	LoginResult()
	{
		m_Size = sizeof( LoginResult );
		m_Type = PKT_SC_LOGIN;
		m_PlayerId = -1;
		memset( m_Name, 0, MAX_NAME_LEN );
	}

	int		m_PlayerId;
	char	m_Name[MAX_NAME_LEN];

};

struct ChatBroadcastRequest: public PacketHeader
{
	ChatBroadcastRequest()
	{
		m_Size = sizeof( ChatBroadcastRequest );
		m_Type = PKT_CS_CHAT;
		m_PlayerId = -1;

		memset( m_Chat, 0, MAX_CHAT_LEN );
	}

	int	m_PlayerId;
	char m_Chat[MAX_CHAT_LEN];
};

struct ChatBroadcastResult: public PacketHeader
{
	ChatBroadcastResult()
	{
		m_Size = sizeof( ChatBroadcastResult );
		m_Type = PKT_SC_CHAT;
		m_PlayerId = -1;

		memset( m_Name, 0, MAX_NAME_LEN );
		memset( m_Chat, 0, MAX_CHAT_LEN );
	}

	int	m_PlayerId;
	char m_Name[MAX_NAME_LEN];
	char m_Chat[MAX_CHAT_LEN];
};

// test Request packet
struct GameOverRequest: public PacketHeader
{
	GameOverRequest( )
	{
		m_Size = sizeof( GameOverRequest );
		m_Type = PKT_CS_GAMEOVER;
		m_PlayerId = -1;
		m_IsWon = false;
	}

	int	m_PlayerId;
	bool m_IsWon;
};

struct GameOverResult: public PacketHeader
{
	GameOverResult( )
	{
		m_Size = sizeof( GameOverResult );
		m_Type = PKT_SC_GAMEOVER;
		m_PlayerId = -1;
		m_IsWon = false;
	}

	int	m_PlayerId;
	bool m_IsWon;
};


struct RoomCreateRequest: public PacketHeader
{
	RoomCreateRequest( )
	{
		m_Size = sizeof( RoomCreateRequest );
		m_Type = PKT_CS_ROOM_CREATE;
		m_PlayerId = -1;
	}

	int	m_PlayerId;
};

struct RoomCreateResult: public PacketHeader
{
	RoomCreateResult( )
	{
		m_Size = sizeof( RoomCreateResult );
		m_Type = PKT_SC_ROOM_CREATE;
		m_RoomNumber = -1;
	}

	int	m_RoomNumber;
};



struct RoomChangeRequest: public PacketHeader
{
	RoomChangeRequest( )
	{
		m_Size = sizeof( RoomChangeRequest );
		m_Type = PKT_CS_ROOM_CHANGE;
		m_RoomFrom = -1;
		m_RoomTo = -1;
		m_PlayerId = -1;
	}

	int m_RoomFrom;
	int m_RoomTo;
	int	m_PlayerId;
};

struct RoomChangeResult: public PacketHeader
{
	RoomChangeResult( )
	{
		m_Size = sizeof( RoomChangeResult );
		m_Type = PKT_SC_ROOM_CHANGE;
		m_RoomNumber = -1;
	}

	int	m_RoomNumber;
};


// 사실 request는 테스트용
struct GenerateCorpsRequest: public PacketHeader
{
	GenerateCorpsRequest( )
	{
		m_Size = sizeof( GenerateCorpsRequest );
		m_Type = PKT_CS_GENERATE_CORPS;
		m_UnitType = UnitType::UNIT_NONE;
		m_Position;
	}

	UnitType m_UnitType;
	Position m_Position;
};

struct GenerateCorpsResult: public PacketHeader
{
	GenerateCorpsResult( )
	{
		m_Size = sizeof( GenerateCorpsResult );
		m_Type = PKT_SC_GENERATE_CORPS;
		m_UnitType = UnitType::UNIT_NONE;
		m_Position;
		m_CorpsID = -1;
		m_PlayerId = -1;
	}

	UnitType m_UnitType;
	Position m_Position;
	int		 m_CorpsID;
	int		 m_PlayerId;
};


// 사실 request는 테스트용 이동 아직 안만듬!!!!
struct MoveCorpsRequest: public PacketHeader
{
	MoveCorpsRequest( )
	{
		m_Size = sizeof( MoveCorpsRequest );
		m_Type = PKT_CS_MOVE_CORPS;
		m_CorpsID = -1;
		m_Position;
		
	}

	int		 m_CorpsID;
	Position m_Position;
	
};

struct MoveCorpsResult: public PacketHeader
{
	MoveCorpsResult( )
	{
		m_Size = sizeof( MoveCorpsResult );
		m_Type = PKT_SC_MOVE_CORPS;
		m_PlayerId = -1;
		m_CorpsID = -1;
		m_Position;
		m_Time = -1;
	}

	int			m_PlayerId;
	int			m_CorpsID;
	Position	m_Position;
	int			m_Time;
};



struct ChangeCorpsFormationRequest: public PacketHeader
{
	ChangeCorpsFormationRequest( )
	{
		m_Size = sizeof( ChangeCorpsFormationRequest );
		m_Type = PKT_CS_CORPS_CHANGE_FORMATION;
		m_CorpsID = -1;
		m_FormationType = FormationType::FORMATION_NONE;
	}

	int				m_CorpsID;
	FormationType	m_FormationType;
};

struct ChangeCorpsFormationResult: public PacketHeader
{
	ChangeCorpsFormationResult( )
	{
		m_Size = sizeof( ChangeCorpsFormationResult );
		m_Type = PKT_SC_CORPS_CHANGE_FORMATION;
		m_CorpsID = -1;
		m_FormationType = FormationType::FORMATION_NONE;
	}

	int				m_CorpsID;
	FormationType	m_FormationType;
};

#pragma pack(pop)