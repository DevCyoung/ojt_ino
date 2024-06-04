#include "Header//Struct.fxh"
#include "Header//Texture.fxh"
#include "Header//ConstantBuffer.fxh"

tVSOut main(tVSIn In)
{
	tVSOut output = (tVSOut) 0.f;

	output.Pos = float4(In.Pos * 2.f, 1.f);
	output.UV = In.UV;

	return output;

}