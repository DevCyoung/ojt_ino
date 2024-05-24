#include "Header//Struct.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Header//Sampler.fxh"

float4 main(tVSOut In) : SV_TARGET
{
	float2 uv = In.UV;
	
	uv.y = uv.y - 1.0 + B6_WaveYPower * B4_GlobalTime.x;
	uv.x += sin(uv.y * B6_WaveXPower + B4_GlobalTime.x * B6_WaveSpeed) / 150.0;
	
	float4 color = Rendertarget.Sample(pointSampler, uv);	
	return color;
}