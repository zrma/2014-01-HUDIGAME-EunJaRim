#pragma once

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
	double	m_PosX;
	double	m_PosY;
	double	m_PosZ;
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

#pragma pack(pop)