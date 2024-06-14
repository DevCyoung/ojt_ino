#pragma once

#include <d3d11.h>
#include <wrl/client.h>

enum class eSMType
{
	Default,
	Std3D,
	End,
};


class SMCollection
{
	friend class GraphicDeviceDX11;

private:
	SMCollection();
	virtual ~SMCollection();
	SMCollection(const SMCollection&) = delete;
	SMCollection& operator=(const SMCollection&) = delete;

public:
	size_t GetInputElementCount(const eSMType SMType) const 
	{ 
		return mInputElementsArray[static_cast<UINT>(SMType)].size();
	}

	const D3D11_INPUT_ELEMENT_DESC* GetInputElementDatas (const eSMType SMType) const 
	{ 
		return mInputElementsArray[static_cast<UINT>(SMType)].data();
	}


private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> mInputElementsArray[static_cast<UINT>(eSMType::End)];
};

