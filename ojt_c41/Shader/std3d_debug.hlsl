#ifndef _STD3D_DEBUG
#define _STD3D_DEBUG

#include "Header//Struct.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"

struct VS_IN
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
    
	float3 vNormal : NORMAL;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
	
	float4 vWeights : BLENDWEIGHT;
	float4 vIndices : BLENDINDICES;
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
	float2 vUV : TEXCOORD;
    
	float3 vViewPos : POSITION;
	
	float3 vViewNormal : NORMAL;
	float3 vViewTangent : TANGENT;
	float3 vViewBinormal : BINORMAL;
    
};

struct tSkinningInfo
{
	float3 vPos;
	float3 vTangent;
	float3 vBinormal;
	float3 vNormal;
};

VS_OUT VS_Std3D(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;	
    // 로컬에서의 Normal 방향을 월드로 이동    
	//output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);    
	//output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
	//output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
	//output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;       
	output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;      
	return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
	float4 vOutColor = B2_Color_1;	
	return vOutColor;
}

#endif