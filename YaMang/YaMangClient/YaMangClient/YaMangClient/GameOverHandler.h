#pragma once
#include "PacketType.h"

class GameOverHandler
{
public:
	GameOverHandler();
	~GameOverHandler();

	static void HandleEvent( PacketHeader& pktBase );
};

