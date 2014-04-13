#pragma once
class ClientSession;

class GameOverHandler
{
public:
	GameOverHandler();
	~GameOverHandler();

	static void HandleEvent( ClientSession* session, PacketHeader& pktBase );
};

