#pragma once
#include "RenderComponent.h"
#include "StructBuffer.h"

REGISTER_COMPONENT_TYPE(Light3D);

class Light3D : public Component
{
public:
	enum class LIGHT_TYPE
	{
		DIRECTIONAL,
		POINT,
		SPOT,
	};

public:
	Light3D();
	virtual ~Light3D();
	Light3D(const Light3D&) = delete;
	Light3D& operator=(const Light3D&) = delete;

	const tLight3DInfo& GetLightInfo() const { return mLightInfo; }
	void SetLightInfo(const tLight3DInfo& lightInfo) { mLightInfo = lightInfo; }


private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;


private:
	tLight3DInfo mLightInfo;
};

