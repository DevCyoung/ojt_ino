#include "pch.h"
#include "ConstantBuffer.h"


	ConstantBuffer::ConstantBuffer(const eCBType CBType, const UINT bufferSize, ID3D11Device* const device)
		: mType(CBType)
		, mDesc{}
		, mSize(bufferSize)
		, mBuffer(nullptr)
	{
		Assert(device, ASSERT_MSG_NULL);
		Assert((mSize % 16) == 0, "data not al 16 byte!");		

		mDesc.ByteWidth = mSize;
		mDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		mDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

		device->CreateBuffer(&mDesc, nullptr, mBuffer.GetAddressOf());
		Assert(mBuffer.Get(), ASSERT_MSG_NULL);
	}

	ConstantBuffer::~ConstantBuffer()
	{
	}
