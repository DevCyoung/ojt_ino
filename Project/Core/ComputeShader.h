#pragma once
#include "Resource.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "StructBuffer.h"
#include "Material.h"

class ComputeShader : public Resource
{
    friend class GraphicDeviceDX11;
    friend class ResourceManager;

public:	
    ComputeShader() = default;
	ComputeShader(const std::wstring& CSRelativePath, const std::wstring& CSFunName);
	virtual ~ComputeShader();
	ComputeShader(const ComputeShader&) = delete;
	ComputeShader& operator=(const ComputeShader&) = delete;
    void SetGroupPerThread(UINT x, UINT y, UINT z) { m_iGroupPerThreadX = x, 
        m_iGroupPerThreadY = y, m_iGroupPerThreadZ = z; };
    void Execute();


private:
    Microsoft::WRL::ComPtr<ID3D11ComputeShader>     mCS;
    Microsoft::WRL::ComPtr<ID3DBlob>                mCSBlob;

protected:
    // ���̴��� ������ ��� ������
    tMaterial                       m_Const;

    // �׷� ����
    UINT                            mGroupX;
    UINT                            mGroupY;
    UINT                            mGroupZ;

    // �׷� 1���� ������ ����
    UINT                            m_iGroupPerThreadX;
    UINT                            m_iGroupPerThreadY;
    UINT                            m_iGroupPerThreadZ;


    // Resource��(��) ���� ��ӵ�
    virtual HRESULT Load(const std::wstring& filePath) override;

};