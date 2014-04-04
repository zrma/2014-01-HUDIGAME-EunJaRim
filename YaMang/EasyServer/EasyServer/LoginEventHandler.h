#pragma once
#include "EventHandler.h"

class LoginEventHandler : public EventHandler
{
public:
	LoginEventHandler();
	virtual ~LoginEventHandler();
	
	virtual void ReadData( CircularBuffer* mRecvBuffer, LoginRequest* inPacket, PacketHeader* header );
	virtual void HandleEvent( SOCKET* mSocket, LoginRequest* inPacket, PacketHeader* header );

};

