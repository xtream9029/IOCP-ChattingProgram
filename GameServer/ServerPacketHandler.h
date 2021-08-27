#pragma once

enum
{
	S_CHAT = 1,
	C_CHAT = 2
};

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static void Handle_C_CHAT(BYTE* buffer, int32 len);

	static SendBufferRef Make_S_CHAT(wstring chat);
};

