#pragma once

struct PacketHeader;
class ClientSession;
class CircularBuffer;

///# __interface는 표준이 아닌거 알고 쓰는거쟈?
__interface EventHandler
{
public:
	///# client 객체 넘기는데 CircularBuffer* buffer, SOCKET* socket 왜 같이 넘기는가? 그럴필요 없자나.. 다 클라 안에 포함된 놈인디..
	virtual void HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket ) = 0;
};