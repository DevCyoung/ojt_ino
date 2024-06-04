//Unorderd Acces View (UAV) : 텍스처를 읽고 쓰기 위한 뷰
RWTexture2D<unorm float4> src : register(u0);
RWTexture2D<unorm float4> dst : register(u1);

//https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/sv-dispatchthreadid
// SV_GroupID           : 스레드가 속한 그룹의 좌표
// SV_GoupThreadID      : 그룹 내에서, 스레드의 좌표
// SV_GoupIndex         : 그룹 내에서, 스레드의 인덱스 (1차원)
// SV_DispatchThreadID  : 전체 스레드(모든 그룹 통합) 기준으로, 호출된 스레드의 좌표

#include "Header//ConstantBuffer.fxh"

#define width  g_int_0
#define height g_int_1
#define alpha g_float_0

[numthreads(32, 32, 1)] // 그룹 당 스레드 개수 (최대 1024 까지 지정 가능)
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
