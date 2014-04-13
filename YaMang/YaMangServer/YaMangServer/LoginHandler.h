#pragma once
class ClientSession;

class LoginHandler
{
public:
	LoginHandler();
	~LoginHandler();

	static void HandleEvent( ClientSession* session, PacketHeader& pktBase );
};

