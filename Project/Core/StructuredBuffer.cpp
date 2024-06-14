#include "pch.h"
#include "StructuredBuffer.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"

StructuredBuffer::StructuredBuffer(const eSBType SBType,
	const eSRVTpye SRVType,
	const UINT elementSize,
	const UINT elementCount,
	const void* const dataOrNull, ID3D11Device* const device)
	: mType(SBType)
	, mSRVType(SRVType)
	, mElementSize(elementSize)
	, mElementCount(elementCount)
	, mDesc{}
	, mBuffer(nullptr)
	, mSRV(nullptr)
	, m_UAV(nullptr)
{
	// 16바이트 단위 메모리 정렬
	Assert((mElementSize % 16) == 0, ASSERT_MSG_INVALID);
	Assert(device, ASSERT_MSG_NULL);

	//FIXME
	mDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE  | D3D11_BIND_UNORDERED_ACCESS;
	mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	//mDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	mDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	mDesc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	mDesc.ByteWidth = mElementSize * mElementCount;
	mDesc.StructureByteStride = mElementSize; // 데이터 간격	

	if (dataOrNull)
	{
		D3D11_SUBRESOURCE_DATA SUBData = {};
		SUBData.pSysMem = dataOrNull;
		HRESULT hr = device->CreateBuffer(&mDesc, &SUBData, mBuffer.GetAddressOf());
		Assert(SUCCEEDED(hr), ASSERT_MSG_INVALID);		
	}
	else
	{		
		HRESULT hr = device->CreateBuffer(&mDesc, nullptr, mBuffer.GetAddressOf());
		Assert(SUCCEEDED(hr), ASSERT_MSG_INVALID);
	}
	
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.BufferEx.NumElements = mElementCount;
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION::D3D_SRV_DIMENSION_BUFFEREX;
		HRESULT hr = device->CreateShaderResourceView(mBuffer.Get(), &SRVDesc, mSRV.GetAddressOf());
		Assert(SUCCEEDED(hr), ASSERT_MSG_INVALID);
	}	

	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = mElementCount;
		HRESULT hr = device->CreateUnorderedAccessView(mBuffer.Get(), &UAVDesc, m_UAV.GetAddressOf());
		Assert(SUCCEEDED(hr), ASSERT_MSG_INVALID);
	}
}

StructuredBuffer::StructuredBuffer(const UINT dataSize,
	const UINT stride,
	const void* const dataOrNull)
	: StructuredBuffer(eSBType::Custom, 
		eSRVTpye::Custom, 
		dataSize, 
		stride,
		dataOrNull, 
		gGraphicDevice->UnSafe_GetDevice())
{
}

StructuredBuffer::~StructuredBuffer()
{
}
