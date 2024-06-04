#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"
#include "ColorFun.hlsl"

float4 main(tVSIn In) : SV_TARGET
{
	float4 color = (float) 0.0f;

	color = MeshRenderer00.Sample(pointSampler, In.UV);
	color.a = 1.f;
	
	return color;
}
