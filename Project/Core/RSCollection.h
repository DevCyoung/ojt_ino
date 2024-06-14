#pragma once
#include <d3d11.h>
#include <wrl/client.h>

enum class eRSType
{
	CullBack,
	CullFront,
	CullNone,
	WireframeNone,
	End,
};

struct ID3D11Device;


class RSCollection
{
	friend class GraphicDeviceDX11;
private:
	RSCollection(ID3D11Device* const device);
	virtual ~RSCollection();
	RSCollection(const RSCollection&) = delete;
	RSCollection& operator=(const RSCollection&) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRStates[static_cast<UINT>(eRSType::End)];
};