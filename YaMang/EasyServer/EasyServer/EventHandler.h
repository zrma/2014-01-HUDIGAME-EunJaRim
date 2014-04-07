﻿#pragma once
#include "ClientSession.h"
__interface EventHandler
{
public:
	virtual void HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket ) = 0;
};