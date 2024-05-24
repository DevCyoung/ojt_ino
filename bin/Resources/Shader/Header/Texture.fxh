#ifndef TEXTURE_FXHH
#define TEXTURE_FXHH

Texture2D MeshRenderer00 : register(t0);
Texture2D MeshRendererNormal : register(t1);
Texture2D MeshRendererSpecul : register(t2);


Texture2D Noise01 : register(t7);
Texture2D ShadowMap00 : register(t8);
Texture2D Rendertarget : register(t10);
Texture2D AtlasAnimation2D : register(t11);

struct tLightColor
{
	float4 vDiffuse; // 빛의 색상
	float4 vAmbient; // 주변 광(환경 광)
	float4 vSpecular; // 반사광 세기
};

struct tLightInfo
{
	tLightColor Color; // 빛의 색상
    
	float4 vWorldPos; // 광원의 월드 스페이스 위치
	float4 vWorldDir; // 빛을 보내는 방향
    
	uint LightType; // 빛의 타입(방향성, 점, 스포트)
	float Radius; // 빛의 반경(사거리)
	float Angle; // 빛의 각도    
	int Padding;
};

StructuredBuffer<tLightInfo> g_Light2DBuffer : register(t13);
StructuredBuffer<tLightInfo> g_ParticleBuffer : register(t15);

#endif	