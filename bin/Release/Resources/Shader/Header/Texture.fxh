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
	float4 vDiffuse; // ���� ����
	float4 vAmbient; // �ֺ� ��(ȯ�� ��)
	float4 vSpecular; // �ݻ籤 ����
};

struct tLightInfo
{
	tLightColor Color; // ���� ����
    
	float4 vWorldPos; // ������ ���� �����̽� ��ġ
	float4 vWorldDir; // ���� ������ ����
    
	uint LightType; // ���� Ÿ��(���⼺, ��, ����Ʈ)
	float Radius; // ���� �ݰ�(��Ÿ�)
	float Angle; // ���� ����    
	int Padding;
};

StructuredBuffer<tLightInfo> g_Light2DBuffer : register(t13);
StructuredBuffer<tLightInfo> g_ParticleBuffer : register(t15);

#endif	