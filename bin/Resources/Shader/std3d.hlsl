#ifndef _STD3D
#define _STD3D

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


// Animation3D Bone Matrix Buffer
StructuredBuffer<Matrix> g_arrBoneMat : register(t30);

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
	return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

struct tSkinningInfo
{
	float3 vPos;
	float3 vTangent;
	float3 vBinormal;
	float3 vNormal;
};

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
	tSkinningInfo info = (tSkinningInfo) 0.f;

	if (_iRowIdx == -1)
		return;
	for (int i = 0; i < 4; ++i)
	{
		if (0.f == _vWeight[i])
			continue;

		matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

		info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
		info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
		info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
		info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
	}

	_vPos = info.vPos;
	_vTangent = normalize(info.vTangent);
	_vBinormal = normalize(info.vBinormal);
	_vNormal = normalize(info.vNormal);
}


VS_OUT VS_Std3D(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;	
	
	if (g_bAnim)
	{
		Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
	}
	
    // 로컬에서의 Normal 방향을 월드로 이동    
    output.vViewPos = mul(float4(_in.vPos, 1.f), B0_WV);
    
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), B0_WV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), B0_WV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), B0_WV)).xyz;
       
    output.vPosition = mul(float4(_in.vPos, 1.f), B0_WVP);
	output.vUV = _in.vUV;
      
	return output;
}

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
	float4 albedo = float4(0.5f, 0.5f, 0.5f, 1.f);        
	float3 viewNormal = _in.vViewNormal;	
		
	if (g_btex_0)
	{
		albedo = MeshRenderer00.Sample(anisotropicSampler, _in.vUV);
	}
    
	if (g_btex_1)
	{
		float3 normal = MeshRendererNormal.Sample(anisotropicSampler, _in.vUV).xyz;
        
        // 0 ~ 1 범위의 값을 -1 ~ 1 로 확장        
		normal = normal * 2.f - 1.f;
        
		float3x3 vRotateMat =
		{	
			_in.vViewTangent,
            -_in.vViewBinormal,
            _in.vViewNormal
		};
        
		viewNormal = mul(normal, vRotateMat);
	}
	
	float3 viewLightDir = normalize(mul(float4(normalize(LIGHT_DIR.xyz), 0.f), B0_ViewInv)).xyz; //광원의 방향
	float  diffuseInstensity = saturate(dot(viewNormal, -viewLightDir)); // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)
	if (g_idebug0 == 1)
	{
		diffuseInstensity = ceil(diffuseInstensity * 5) / 5.f;
	}
	
		    
    // 반사광
	float3 viewReflectDir = normalize(viewLightDir + 2.f * (dot(-viewLightDir, viewNormal)) * viewNormal);
	float3 viewEyeDir = normalize(_in.vViewPos);
   
    // 반사광의 세기 구하기	
	float specularPow = pow(saturate(dot(viewReflectDir, -viewEyeDir)), 9.f);
           
	float3 diffuse  = LIGHT_COLOR.rgb * albedo.rgb * diffuseInstensity;	
	float3 ambient  = LIGHT_COLOR.rgb * albedo.rgb * LIGHT_AMBIENT.rgb;
	float3 specular = LIGHT_COLOR.rgb * LIGHT_SPEC_COEFF * specularPow;
	
	//FIXME Specular 지멋대로인거 고치기
	if (g_btex_2)
	{
		const float3 specularInstensity = MeshRendererSpecul.Sample(anisotropicSampler, _in.vUV).rgb;		
		specular = LIGHT_COLOR.rgb * specularInstensity * specularPow * 2.5f;
	}
	
	return float4(diffuse + specular + ambient, 1.f);
}
#endif