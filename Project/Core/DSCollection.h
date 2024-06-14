#pragma once
#include <d3d11.h>
#include <wrl/client.h>

enum class eDSType
{
	Less,	//test winer less, Write O
	LessEqual, //test winer lessequal, Write O
	Greater , //test winer Greater, Write O
	NoWrite, //test winer less, Write X
	None, //test X, write X 무조건 덧그림
	End,
};

struct ID3D11Device;

class DSCollection
{
	friend class GraphicDeviceDX11;
private:
	DSCollection(ID3D11Device* const device);
	virtual ~DSCollection();
	DSCollection(const DSCollection&) = delete;
	DSCollection& operator=(const DSCollection&) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDStates[static_cast<UINT>(eDSType::End)];
};
