#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

float4 main(tVSOut In) : SV_TARGET
{
	float4 color = (float) 0.0f;
	
	float2 uv = In.UV;
	
	float angle = 45 * (3.14f / 180.f);
	

	uv.x = In.UV.x * cos(angle) + In.UV.y * -sin(angle);
	uv.y = In.UV.x * sin(angle) + In.UV.y * cos(angle);

	if (frac(uv.x) < B5_A || frac(uv.x) > 1 - B5_A)
	{
		discard;
	}
	
	if (frac(uv.y) < B5_A || frac(uv.y) > 1 - B5_A)
	{
		discard;
	}
	
	color = MeshRenderer00.Sample(pointSampler, uv);
	
	if (color.r == 1.0f && color.g == 0.0f && color.b == 1.0f)
	{
		discard;
	}
	else if (color.a == 0.f)
	{
		discard;
	}
	
	

	return color;
}










//float4 main(tVSOut In) : SV_TARGET
//{
//	float4 color = float4(1.f, 1.f, 1.f, 1.f);
//	float2 uv = In.UV;
//	float angle = 45 * (3.14f / 180.f);
	
//	uv.x = In.UV.x * cos(angle) + In.UV.y * -sin(angle);
//	uv.y = In.UV.x * sin(angle) + In.UV.y *  cos(angle);	
	
//	return color;
//}
