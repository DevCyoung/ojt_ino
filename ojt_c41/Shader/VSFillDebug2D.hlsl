#include "Header//Struct.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Header//TestShaderHeader.fxh"


tVSOut main(tVSIn In)
{
	tVSOut Out = (tVSOut) 0.0f;
    
	const float4 WORLD = mul(float4(In.Pos, 1.0f), B0_World);
	const float4 VIEW = mul(WORLD, B0_View);
	const float4 PROJ = mul(VIEW, B0_Projection);
    
	Out.Pos = PROJ;
	Out.UV = In.UV;
	Out.Color = In.Color;
	
	return Out;
}