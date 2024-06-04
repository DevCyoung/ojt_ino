#include "pch.h"
#include "Protocal.h"

void serializeData(const void* ori, size_t dataSize, void* packetBuffer)
{
	std::memcpy(packetBuffer, ori, dataSize);
}

// ������ȭ �Լ�
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori)
{
	std::memcpy(ori, packetBuffer, dataSize);
}

// ��Ŷ ID�� Ȯ���ϴ� �Լ�
int getPacketId(char(&buffer)[], int recvSize) {
	// ���۰� ��� ũ�⺸�� ������ ��ȿ�� ��Ŷ�� �ƴ�
	if (recvSize < sizeof(int))
	{
		return -1; // 0�� ��ȿ���� ���� ID�� �����մϴ�.
	}

	// ��Ŷ ������� ��Ŷ ID�� ����
	//PacketHeader header;
	int header = 0;
	std::memcpy(&header, buffer, sizeof(int));
	return header;
}