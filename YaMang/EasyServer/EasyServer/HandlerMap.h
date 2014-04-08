#pragma once
#include <map>
#include "MacroSet.h"

__interface EventHandler;
struct PacketHeader;
class ClientSession;
class CircularBuffer;

///# 사실 핸들러 테이블 같은거는 싱글톤으로 만드는거 좋지 않다. 왜냐하면 매번 핸들러 호출할 때마다 GetInstane()를 호출해야 하니까.. 사실상 CPU 명령 더 먹는거...
///# EasyServer 최신버전 static 전역 배열 등록해놓은거 참고.
class HandlerMap : public Singleton<HandlerMap>
{
public:
	HandlerMap();
	~HandlerMap();

	bool	HandleEvent( short* packetType, ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );

private:
	std::map<short, EventHandler*> mPacketHandler;

};

