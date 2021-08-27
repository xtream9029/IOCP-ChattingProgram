#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSessionManager.h"


void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

	switch (header.id)
	{
	case C_CHAT:
		Handle_C_CHAT(buffer, len);
		break;
	default:
		break;
	}
}

void ServerPacketHandler::Handle_C_CHAT(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	wstring name;
	uint16 nameLen;
	br >> nameLen;
	name.resize(nameLen);
	br.Read((void*)name.data(), nameLen * sizeof(WCHAR));

	wstring chat;
	uint16 chatLen;
	br >> chatLen;
	chat.resize(chatLen);
	br.Read((void*)chat.data(), chatLen * sizeof(WCHAR));

	wstring Chat=L"";
	Chat += name;
	Chat += ':';
	Chat += chat;

	SendBufferRef sendBuffer = ServerPacketHandler::Make_S_CHAT(Chat);
	GSessionManager.Broadcast(sendBuffer);
}

SendBufferRef ServerPacketHandler::Make_S_CHAT(wstring chat)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());

	PacketHeader* header = bw.Reserve<PacketHeader>();

	//CHAT
	bw << (uint16)chat.size();
	bw.Write((void*)chat.data(), chat.size() * sizeof(WCHAR));

	header->size = bw.WriteSize();
	header->id = S_CHAT; 

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}
