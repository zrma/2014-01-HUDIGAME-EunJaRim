#pragma once

class ClientSession;

class ChatHandler
{
public:
	ChatHandler();
	~ChatHandler();

	static void HandleEvent( ClientSession* session, PacketHeader& pktBase );
};

