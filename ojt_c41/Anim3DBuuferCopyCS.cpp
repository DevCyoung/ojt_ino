#include "pch.h"
#include "Anim3DBuuferCopyCS.h"
#include "StructuredBuffer.h"

#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "EnumShaderBindStage.h"
#include "ConstantBuffer.h"

Anim3DBuuferCopyCS::Anim3DBuuferCopyCS(const std::wstring& CSRelativePath, const std::wstring& CSFunName)
	: ComputeShader(CSRelativePath, CSFunName)
	, m_pFrameDataBuffer(nullptr)
	, m_pOffsetMatBuffer(nullptr)
	, m_pOutputBuffer(nullptr)
{
}

Anim3DBuuferCopyCS::~Anim3DBuuferCopyCS()
{
	//delete m_pFrameDataBuffer;
	//m_pFrameDataBuffer = nullptr;
	//delete m_pOffsetMatBuffer;
	//m_pOffsetMatBuffer = nullptr;
	//delete m_pOutputBuffer;
	//m_pOutputBuffer = nullptr;	
}

void Anim3DBuuferCopyCS::UpdateData()
{
	Assert(m_pFrameDataBuffer, ASSERT_MSG_NULL);
	Assert(m_pOffsetMatBuffer, ASSERT_MSG_NULL);
	Assert(m_pOutputBuffer, ASSERT_MSG_NULL);

	gGraphicDevice->BindSB(16,  m_pFrameDataBuffer, eShaderBindType::CS); // t16
	gGraphicDevice->BindSB(17,  m_pOffsetMatBuffer, eShaderBindType::CS); // t17
	gGraphicDevice->BindSBCS(0, m_pOutputBuffer);						  // u0

	mGroupX = (m_Const.arrInt[0] / m_iGroupPerThreadX) + 1;
	mGroupY = 1;
	mGroupZ = 1;
	
	gGraphicDevice->PassCB(eCBType::Material, sizeof(m_Const), &m_Const);
	gGraphicDevice->BindCB(eCBType::Material, eShaderBindType::CS);

	//gGraphicDevice->Distpatch(this)
}

void Anim3DBuuferCopyCS::Clear()
{	
	ID3D11ShaderResourceView* pSRV = nullptr;
	gGraphicDevice->UnSafe_Context()->CSSetShaderResources(16, 1, &pSRV);
	gGraphicDevice->UnSafe_Context()->CSSetShaderResources(17, 1, &pSRV);

	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = UINT_MAX;
	gGraphicDevice->UnSafe_Context()->CSSetUnorderedAccessViews(0, 1, &pUAV, &i);
}
