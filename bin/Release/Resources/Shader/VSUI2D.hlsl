#include "Header//Struct.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Header//TestShaderHeader.fxh"


tVSOut main(tVSIn In)
{
	tVSOut Out = (tVSOut) 0.0f;
    
	float4 world = mul(float4(In.Pos, 1.0f), B0_World);
	float4 view = mul(world, B0_View);
	float4 proj = mul(view, B0_Projection);
    
	Out.Pos = proj;
	Out.UV = In.UV;
	Out.Color = In.Color;
	
	return Out;
}