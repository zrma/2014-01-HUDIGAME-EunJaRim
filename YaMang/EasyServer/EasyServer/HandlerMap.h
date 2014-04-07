#pragma once
#include <map>
#include "MacroSet.h"

__interface EventHandler;
struct PacketHeader;
class ClientSession;
class CircularBuffer;

class HandlerMap : public Singleton<HandlerMap>
{
public:
	HandlerMap();
	~HandlerMap();

	bool	HandleEvent( short* packetType, ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );

private:
	std::map<short, EventHandler*> mPacketHandler;

};

