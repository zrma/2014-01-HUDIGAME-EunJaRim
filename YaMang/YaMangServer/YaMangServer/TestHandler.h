#pragma once
#include "ClientSession.h"

class TestHandler
{
public:
	TestHandler();
	~TestHandler();

	static void HandleEvent( ClientSession* session, PacketHeader& pktBase );

};

