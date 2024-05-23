#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Light2D.hlsl"
#include "PSAnimation2D.hlsl"

float4 main(tVSLightOut In) : SV_TARGET
{
	float4 outColor = animationColor(In.vUV);
			
	if (B1_bUseColor == 1)
	{
		float src = 0.7f;
		float dst = 1.f - src;
		
		outColor.r = 0.0f;
		outColor.g = outColor.g + outColor.g * src + 1.f * dst;
		outColor.b = outColor.b + outColor.b * src + 1.f * dst;
		outColor.a = B1_cbColor.a;
	}
	
	tLightColor LightColor = (tLightColor) 0.f;    
	CalcLight2D(In.vWorldPos, LightColor);
	
	outColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);
	
	if (outColor.a <= 0.f)
	{
		discard;
	}
	
	return outColor;
}