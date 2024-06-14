#pragma once

#include "StructuredBuffer.h"

class SBCollection
{
	friend class GraphicDeviceDX11;
private:
	SBCollection(ID3D11Device* const device);
	virtual ~SBCollection();
	SBCollection(const SBCollection&) = delete;
	SBCollection& operator=(const SBCollection&) = delete;

public:
	StructuredBuffer* GetStructuredBuffer(const eSBType SBType) const
	{
		return mStructuredBuffers[static_cast<UINT>(SBType)];
	}

	void ReSizeStructuredBuffer(const eSBType SBType,
		const eSRVTpye SRVType,
		const UINT dataSize,
		const UINT stride,
		const void* const dataOrNull, 
		ID3D11Device* const device);

	StructuredBuffer* mStructuredBuffers[static_cast<UINT>(eSBType::End)];
};

