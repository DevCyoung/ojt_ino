#pragma once
#include "ScriptComponent.h"
#include "EnumScriptComponent.h"

REGISTER_SCRIPTCOMPONENT_TYPE(CameraInputMoveMent);

class CameraInputMoveMent : public ScriptComponent
{
public:
	CameraInputMoveMent();
	virtual ~CameraInputMoveMent();
	//CameraInputMoveMent(const CameraInputMoveMent&) = delete;
	CameraInputMoveMent& operator=(const CameraInputMoveMent&) = delete;
	void MoveCamera();
	CLONE(CameraInputMoveMent)
private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;

};
