#pragma once
#include <map>
#include "ClientSession.h"
#include "..\YaMangClient\MacroSet.h"

class HandlerMap : public Singleton<HandlerMap>
{
public:
	HandlerMap();
	~HandlerMap();

	bool	HandleEvent( short* packetType, ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );

private:
	std::map<short, void( *)( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket )> mPacketHandler;

};

