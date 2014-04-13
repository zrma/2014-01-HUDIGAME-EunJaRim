#pragma once
class PacketHeader;

class ChatHandler
{
public:
	ChatHandler();
	~ChatHandler();

	static void HandleEvent( PacketHeader& pktBase );
};

