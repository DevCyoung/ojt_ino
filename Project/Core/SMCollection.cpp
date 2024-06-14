#include "pch.h"
#include "SMCollection.h"

SMCollection::SMCollection()
{
	for (auto& mInputElements : mInputElementsArray)
	{
		mInputElements.reserve(10);
	}

	//Default
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC>& DeafultIEDArray =
			mInputElementsArray[static_cast<UINT>(eSMType::Default)];

		constexpr UINT MAX_INPUT_ELEMENT = 3;
		DeafultIEDArray.resize(MAX_INPUT_ELEMENT);

		DeafultIEDArray[0].AlignedByteOffset = 0;
		DeafultIEDArray[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		DeafultIEDArray[0].InputSlot = 0;
		DeafultIEDArray[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		DeafultIEDArray[0].SemanticName = "POSITION";
		DeafultIEDArray[0].SemanticIndex = 0;

		DeafultIEDArray[1].AlignedByteOffset = 12;
		DeafultIEDArray[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		DeafultIEDArray[1].InputSlot = 0;
		DeafultIEDArray[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		DeafultIEDArray[1].SemanticName = "COLOR";
		DeafultIEDArray[1].SemanticIndex = 0;

		DeafultIEDArray[2].AlignedByteOffset = 28;
		DeafultIEDArray[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		DeafultIEDArray[2].InputSlot = 0;
		DeafultIEDArray[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		DeafultIEDArray[2].SemanticName = "TEXCOORD";
		DeafultIEDArray[2].SemanticIndex = 0;
	}

	//Std3D
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC>& LayoutDesc =
			mInputElementsArray[static_cast<UINT>(eSMType::Std3D)];		

		constexpr UINT MAX_INPUT_ELEMENT = 8;
		LayoutDesc.resize(MAX_INPUT_ELEMENT);

		LayoutDesc[0].SemanticName = "POSITION";
		LayoutDesc[0].SemanticIndex = 0;
		LayoutDesc[0].InputSlot = 0;
		LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[0].InstanceDataStepRate = 0;
		LayoutDesc[0].AlignedByteOffset = 0;
		LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;


		LayoutDesc[1].SemanticName = "COLOR";
		LayoutDesc[1].SemanticIndex = 0;
		LayoutDesc[1].InputSlot = 0;
		LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[1].InstanceDataStepRate = 0;
		LayoutDesc[1].AlignedByteOffset = 12;
		LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		LayoutDesc[2].SemanticName = "TEXCOORD";
		LayoutDesc[2].SemanticIndex = 0;
		LayoutDesc[2].InputSlot = 0;
		LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[2].InstanceDataStepRate = 0;
		LayoutDesc[2].AlignedByteOffset = 28;
		LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;

		LayoutDesc[3].SemanticName = "NORMAL";
		LayoutDesc[3].SemanticIndex = 0;
		LayoutDesc[3].InputSlot = 0;
		LayoutDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[3].InstanceDataStepRate = 0;
		LayoutDesc[3].AlignedByteOffset = 36;
		LayoutDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[4].SemanticName = "TANGENT";
		LayoutDesc[4].SemanticIndex = 0;
		LayoutDesc[4].InputSlot = 0;
		LayoutDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[4].InstanceDataStepRate = 0;
		LayoutDesc[4].AlignedByteOffset = 48;
		LayoutDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[5].SemanticName = "BINORMAL";
		LayoutDesc[5].SemanticIndex = 0;
		LayoutDesc[5].InputSlot = 0;
		LayoutDesc[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[5].InstanceDataStepRate = 0;
		LayoutDesc[5].AlignedByteOffset = 60;
		LayoutDesc[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		LayoutDesc[6].SemanticName = "BLENDWEIGHT";
		LayoutDesc[6].SemanticIndex = 0;
		LayoutDesc[6].AlignedByteOffset = 72;
		LayoutDesc[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[6].InputSlot = 0;
		LayoutDesc[6].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[6].InstanceDataStepRate = 0;

		LayoutDesc[7].SemanticName = "BLENDINDICES";
		LayoutDesc[7].SemanticIndex = 0;
		LayoutDesc[7].AlignedByteOffset = 88;
		LayoutDesc[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc[7].InputSlot = 0;
		LayoutDesc[7].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc[7].InstanceDataStepRate = 0;
	}
}

SMCollection::~SMCollection()
{
}
