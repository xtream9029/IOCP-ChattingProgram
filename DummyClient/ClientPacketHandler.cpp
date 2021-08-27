#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case S_CHAT:
		Handle_S_CHAT(buffer, len);
		break;
	default:
		break;
	}
}

struct S_CHAT {
	uint32 id;
	wstring chat;
};

struct C_CHAT {
	wstring name;
	wstring chat;
};

void ClientPacketHandler::Handle_S_CHAT(BYTE* buffer, int32 len)
{
	//서버에서 브로드 캐스팅한 채팅 내용을 출력
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	wstring chat;
	uint16 chatLen;
	br >> chatLen;
	chat.resize(chatLen);

	br.Read((void*)chat.data(), chatLen * sizeof(WCHAR));
	//wcout.imbue(std::locale("kor"));
	wcout << chat << endl;
}

SendBufferRef ClientPacketHandler::Make_C_CHAT(wstring name, wstring chat)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());

	PacketHeader* header = bw.Reserve<PacketHeader>();
	
	//NAME
	bw << (uint16)name.size();
	bw.Write((void*)name.data(), name.size() * sizeof(WCHAR));

	//CHAT
	bw << (uint16)chat.size();
	bw.Write((void*)chat.data(), chat.size() * sizeof(WCHAR));

	header->size = bw.WriteSize();
	header->id = C_CHAT;

	sendBuffer->Close(bw.WriteSize());

	return sendBuffer;
}
