#pragma once

enum
{
	S_CHAT=1,
	C_CHAT=2
};

class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static void Handle_S_CHAT(BYTE* buffer, int32 len);

	static SendBufferRef Make_C_CHAT(wstring name,wstring chat);

};

