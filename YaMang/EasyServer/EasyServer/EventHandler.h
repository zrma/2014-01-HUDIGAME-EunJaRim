#pragma once
#include "CircularBuffer.h"
#include "..\..\PacketType.h"

class EventHandler
{
public:
	virtual void ReadData( CircularBuffer* mRecvBuffer, LoginRequest* inPacket, PacketHeader* header ) = 0;
	virtual void HandleEvent( SOCKET* mSocket, LoginRequest* inPacket, PacketHeader* header ) = 0;
};