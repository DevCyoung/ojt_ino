#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"
#include "ColorFun.hlsl"


float4 main(tVSOut In) : SV_TARGET
{
	float4 color = (float) 0.0f;
	color = MeshRenderer00.Sample(pointSampler, In.UV);
	
	if (color.r == 1.0f && color.g == 0.0f && color.b == 1.0f)
	{
		discard;
	}
	else if (color.a == 0.f)
	{
		discard;
	}
	
	color = setColor(color);
	color = mulColor(color);	
	
	
	if (In.UV.x > B5_SpriteUV.x)
	{
		discard;
	}
	
	return color;
}