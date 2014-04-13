#pragma once
struct PacketHeader;

class LoginHandler
{
public:
	LoginHandler();
	~LoginHandler();

	static void HandleEvent( PacketHeader& pktBase );
};

