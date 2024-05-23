#ifndef SHADOW_MAP_HLSL
#define SHADOW_MAP_HLSL

#include "Header//Struct.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

struct VS_IN
{
	float3 vPos : POSITION;
};

struct VS_SHADOW_OUT
{
	float4 vPosition : SV_Position;
	float4 vProjPos : POSITION;
};

VS_SHADOW_OUT VS_ShadowMap(VS_IN _in)
{
	VS_SHADOW_OUT output = (VS_SHADOW_OUT) 0.f;
    
    // 사용하는 메쉬가 RectMesh(로컬 스페이스에서 반지름 0.5 짜리 정사각형)
    // 따라서 2배로 키워서 화면 전체가 픽셀쉐이더가 호출될 수 있게 한다.
	output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        
	output.vProjPos = output.vPosition;
	output.vProjPos.xyz /= output.vProjPos.w;

	return output;
}

float4 PS_ShadowMap(VS_SHADOW_OUT _in) : SV_Target
{
	return float4(_in.vProjPos.z, 0.f, 0.f, 0.f);
}



#endif