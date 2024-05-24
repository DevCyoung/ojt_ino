#include "Header//Struct.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Header//TestShaderHeader.fxh"


tVSOut main(tVSIn In)
{
	tVSOut output = (tVSOut) 0.f;

	output.Pos = float4(In.Pos * 2.f, 1.f);
	output.UV = In.UV;
	
	output.UV.x *= 44.f;
	output.UV.y *= 26.f;
	
	return output;
}	