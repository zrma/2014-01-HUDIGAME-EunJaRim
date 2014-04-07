#pragma once

struct PacketHeader;
class ClientSession;
class CircularBuffer;

__interface EventHandler
{
public:
	virtual void HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket ) = 0;
};