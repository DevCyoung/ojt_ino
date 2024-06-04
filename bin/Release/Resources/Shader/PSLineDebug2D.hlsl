#include "Header//Struct.fxh"
#include "Header//Sampler.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

#define FILL_COLOR B2_Color_1

float4 main(tVSOut In) : SV_TARGET
{
	float4 color = FILL_COLOR;

	return color;
}