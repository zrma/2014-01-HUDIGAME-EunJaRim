#pragma once
#include "ClientSession.h"

void ClientLoginPacket( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );
void ClientChatPacket( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );