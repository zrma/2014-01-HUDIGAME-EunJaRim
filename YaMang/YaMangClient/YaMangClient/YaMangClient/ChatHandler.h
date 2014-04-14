#pragma once
struct PacketHeader;

class ChatHandler
{
public:
	ChatHandler();
	~ChatHandler();

	static void HandleEvent( PacketHeader& pktBase );
};

