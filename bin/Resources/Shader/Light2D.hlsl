#include "Header//Struct.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

void CalcLight2DPoint(int i, float3 _vWorldPos, inout tLightColor _Light)
{
	float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
	float3 vWorldPos = float3(_vWorldPos.xy, 0.f);

	float fDistance = abs(distance(vWorldPos, vLightWorldPos));
	float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
        
	_Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow;
}

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
	for (int i = 0; i < B4_Light2DCount; ++i)
	{
		if (g_Light2DBuffer[i].LightType == DIRECTIONAL)
		{
			_Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb;
			_Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
		}
		else if (g_Light2DBuffer[i].LightType == POINT)
		{
			CalcLight2DPoint(i, _vWorldPos, _Light);

		}
		else if (g_Light2DBuffer[i].LightType == SPOT)
		{					
			float2 lightDir = normalize(g_Light2DBuffer[i].vWorldDir.xy);
			float2 targetDir = normalize(_vWorldPos.xy - g_Light2DBuffer[i].vWorldPos.xy);			
			float targetAngle = g_Light2DBuffer[i].Angle / 2.f;			
			float radio = dot(lightDir, targetDir) / cos(targetAngle);					
			
			if (cos(targetAngle) <= dot(lightDir, targetDir))
			{
				CalcLight2DPoint(i, _vWorldPos, _Light);
			}									
		}
	}
}