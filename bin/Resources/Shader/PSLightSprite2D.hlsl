#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Light2D.hlsl"

float4 main(tVSLightOut In) : SV_TARGET
{
	float4 color = (float) 0.0f;
	
	color = MeshRenderer00.Sample(pointSampler, In.vUV);	
	
	if (color.r == 1.0f && color.g == 0.0f && color.b == 1.0f)
	{
		discard;
	}
	else if (color.a == 0.f)
	{
		discard;
	}
	
	// Lighting Ã³¸®
	tLightColor LightColor = (tLightColor) 0.f;    
	CalcLight2D(In.vWorldPos, LightColor);        
	color.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);	
	
	
	return color;
}