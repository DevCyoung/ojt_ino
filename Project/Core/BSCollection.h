#pragma once

#include <d3d11.h>
#include <wrl/client.h>

enum class eBSType
{
	Default,
	AlphaBlend,
	OneZero,
	OneOne,
	End,
};

struct ID3D11Device;

class BSCollection
{
	friend class GraphicDeviceDX11;
private:
	BSCollection(ID3D11Device* const device);
	virtual ~BSCollection();
	BSCollection(const BSCollection&) = delete;
	BSCollection& operator=(const BSCollection&) = delete;

	Microsoft::WRL::ComPtr<ID3D11BlendState> mBStates[static_cast<UINT>(eBSType::End)];
};

