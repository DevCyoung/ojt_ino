#include "Header//Struct.fxh"
#include "Header//ConstantBuffer.fxh"
#include "Header//TestShaderHeader.fxh"


tVSLightOut main(tVSIn In)
{
	tVSLightOut Out = (tVSLightOut) 0.0f;
    
	float4 world = mul(float4(In.Pos, 1.0f), B0_World);
	float4 view = mul(world, B0_View);
	float4 proj = mul(view, B0_Projection);
    
	Out.vPosition = proj;
	Out.vUV = In.UV;
	Out.Color = In.Color;
	Out.vWorldPos = world;	
	
	return Out;
}