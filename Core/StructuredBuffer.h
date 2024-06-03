#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "EnumSRV.h"

enum class eSBType
{
	Light2D,
	Particle2D,
	BoneOffset,
	BoneFrameData,
	BoneFinalBuffer,
	End,
	Custom,
};

class StructuredBuffer
{
	friend class GraphicDeviceDX11;
public:
	StructuredBuffer(const eSBType CBType,
		const eSRVTpye SRVType,
		const UINT elementSize,
		const UINT elementCount,
		const void* const dataOrNull, ID3D11Device* const device);

	StructuredBuffer(const UINT elementSize,
		const UINT elementCount,
		const void* const dataOrNull);

	virtual ~StructuredBuffer();

	StructuredBuffer(const StructuredBuffer&) = delete;
	StructuredBuffer& operator=(const StructuredBuffer&) = delete;

	UINT GetElementSize()	{ return mElementSize; }
	UINT GetElementCount()	{ return mElementCount; }
	UINT GetBufferSize()	{ return mElementSize * mElementCount; }

private:	
	eSBType				mType;
	eSRVTpye			mSRVType;
	UINT				mElementSize;
	UINT				mElementCount;
	D3D11_BUFFER_DESC	mDesc;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				mBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mSRV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   m_UAV;
};
