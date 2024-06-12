#pragma once
#include "ScriptComponent.h"
#include "EnumScriptComponent.h"

REGISTER_SCRIPTCOMPONENT_TYPE(CarController);

class CarController : public ScriptComponent
{
public:
	CarController();
	virtual ~CarController();
	//CameraInputMoveMent(const CameraInputMoveMent&) = delete;
	CarController& operator=(const CarController&) = delete;

	CLONE(CarController);
	void AddWheel(GameObject* wheel) { mWheels.push_back(wheel); }
private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;

	std::vector<GameObject*> mWheels;
};