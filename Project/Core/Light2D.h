#pragma once
#include "Component.h"
#include "StructBuffer.h"

REGISTER_COMPONENT_TYPE(Light2D);

class Light2D : public Component
{
public:
	enum class LIGHT_TYPE
	{
		DIRECTIONAL,
		POINT,
		SPOT,
	};

public:
	Light2D();
	virtual ~Light2D();
	Light2D(const Light2D&) = delete;
	Light2D& operator=(const Light2D&) = delete;


public:
	void SetLightType(LIGHT_TYPE _Type) { m_LightInfo.LightType = (UINT)_Type; }

	void SetLightDiffuse(Vector3 _vDiffuse) 
	{ 
		m_LightInfo.Color.Diffuse.x = _vDiffuse.x;
		m_LightInfo.Color.Diffuse.y = _vDiffuse.y;
		m_LightInfo.Color.Diffuse.z = _vDiffuse.z;
	}

	void SetLightAmbient(Vector3 _vAmbient) 
	{ 
		m_LightInfo.Color.Ambient.x = _vAmbient.x; 
		m_LightInfo.Color.Ambient.y = _vAmbient.y;
		m_LightInfo.Color.Ambient.z = _vAmbient.z;
	}

	void SetRadius(float _Radius) { m_LightInfo.Radius = _Radius; }
	void SetAngle(float _Angle) { mSpotAngle = _Angle; }

	void TurnOnLight() { mbOn = true; }
	void TurnOffLight() { mbOn = false; }

	const tLightInfo& GetLightInfo() { return m_LightInfo; }


private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;


private:
	tLightInfo      m_LightInfo;
	float			mSpotAngle;
	bool			mbOn;

};
