#pragma once
#include "ComputeShader.h"
class StructuredBuffer;

class Anim3DBuuferCopyCS : public ComputeShader
{
private:
	StructuredBuffer* m_pFrameDataBuffer;			// t13
	StructuredBuffer* m_pOffsetMatBuffer;			// t14 
	StructuredBuffer* m_pOutputBuffer;				// u0

public:
	void SetFrameData(int boneCount, int frame, int nextFrame, float ratio)
	{
		m_Const.arrInt[0] = boneCount;
		m_Const.arrInt[1] = frame;
		m_Const.arrInt[2] = nextFrame;
		m_Const.arrInt[3] = 0;
		m_Const.arrFloat[0] = ratio;
	}

	void SetMixFrameData(int boneCount, int frame, int nextFrame, float ratio,
		int frame2, int nextFrame2, float ratio2, float mixRatio)
	{
		SetFrameData(boneCount, frame, nextFrame, ratio);
		m_Const.arrInt[3] = 1;
		m_Const.arrInt[4] = frame2;
		m_Const.arrInt[5] = nextFrame2;
		m_Const.arrFloat[1] = ratio2;
		m_Const.arrFloat[2] = mixRatio;
	}

	void SetFrameDataBuffer(StructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
	void SetOffsetMatBuffer(StructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
	void SetOutputBuffer(StructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }

public:
	virtual void UpdateData();
	virtual void Clear();

	Anim3DBuuferCopyCS() = default;
	Anim3DBuuferCopyCS(const std::wstring& CSRelativePath, const std::wstring& CSFunName);
	virtual ~Anim3DBuuferCopyCS();
	Anim3DBuuferCopyCS(const Anim3DBuuferCopyCS&) = delete;
	Anim3DBuuferCopyCS& operator=(const Anim3DBuuferCopyCS&) = delete;
};

