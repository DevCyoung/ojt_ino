//Unorderd Acces View (UAV) : �ؽ�ó�� �а� ���� ���� ��
RWTexture2D<unorm float4> src : register(u0);
RWTexture2D<unorm float4> dst : register(u1);

//https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/sv-dispatchthreadid
// SV_GroupID           : �����尡 ���� �׷��� ��ǥ
// SV_GoupThreadID      : �׷� ������, �������� ��ǥ
// SV_GoupIndex         : �׷� ������, �������� �ε��� (1����)
// SV_DispatchThreadID  : ��ü ������(��� �׷� ����) ��������, ȣ��� �������� ��ǥ

#include "Header//ConstantBuffer.fxh"

#define width  g_int_0
#define height g_int_1
#define alpha g_float_0

[numthreads(32, 32, 1)] // �׷� �� ������ ���� (�ִ� 1024 ���� ���� ����)
void main(int3 id : SV_DispatchThreadID)
{
	if (width <= id.x || height <= id.y)	
	{
		return;
	}
	
	float4 srcColor = src[id.xy];
	srcColor.a = alpha;
	dst[id.xy] = srcColor;
}
