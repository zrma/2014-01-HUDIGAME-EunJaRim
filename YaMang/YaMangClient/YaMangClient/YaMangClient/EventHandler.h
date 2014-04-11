#pragma once

struct PacketHeader;
class CircularBuffer;

__interface EventHandler
{
public:
	virtual void HandleEvent( PacketHeader* header, CircularBuffer* buffer) = 0;
};