#pragma once
class PacketHeader;

class GameOverHandler
{
public:
	GameOverHandler();
	~GameOverHandler();

	static void HandleEvent( PacketHeader& pktBase );
};

