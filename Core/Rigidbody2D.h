#pragma once
#include "Component.h"

REGISTER_COMPONENT_TYPE(Rigidbody2D);

class Rigidbody2D : public Component
{
public:
	Rigidbody2D();
	virtual ~Rigidbody2D();
	Rigidbody2D(const Rigidbody2D&) = delete;
	Rigidbody2D& operator=(const Rigidbody2D&) = delete;

public:
    void AddForce(Vector2 force) { mForce += force; }
    void SetVelocityLimit(float velocityLimit) { mVelocityLimit = velocityLimit; }
    void SetGravityVelocityLimit(float gravityVelocityLimit) { mGravityLimit = gravityVelocityLimit; }

    float GetGravityScale() { return mGravityAccel; }
    float GetGravityVelocityLimit() const { return mGravityLimit; }
    Vector2 GetDirection()  const  { Vector2 dir = mVelocity; dir.Normalize(); return dir; };

    void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
    Vector2 GetVelocity() { return mVelocity; }

    bool IsGround() const { return mbGround; }

    void AddVelocity(Vector2 velocity) { mVelocity += velocity; }
    bool IsStop() const { return mVelocity == Vector2::Zero; }

    void SetFriction(float friction) { mFriction = friction; }
    void SetFrictionScale(float frictionScale) { mFrictionScale = frictionScale; }
    void SetGravityAccel(float accel) { mGravityAccel = accel; }

    bool IsGravity() { return mbGravityUse; }

    void TurnOnGravity() { mbGravityUse = true; }
    void TurnOffGravity() { mbGravityUse = false; }

    void TurnOnGround() { mbGround = true; }
    void TurnOffGround() { mbGround = false; }     

private:
	virtual void initialize() override final;
	virtual void update() override final;
    virtual void fixedUpdate() override final;
	virtual void lateUpdate() override final;

private:
    Vector2    mForce;          // 힘
    Vector2    mVelocity;       // 속도 ( vector )
    float      mMass;           // 질량

    float   mFriction;          // 마찰력
    float   mFrictionScale;     // 마찰계수

    float   mVelocityLimit;     // 제한 속도
    float   mGravityLimit;      // 중력에 의한 제한 속도

    float   mGravityAccel;      // 중력 가속도 설정
    bool    mbGravityUse;       // 중력 사용여부
    bool    mbGround;           // 땅 체크    

    bool mbFixedUpdate;      // FixedUpdate에서만 사용할 경우 true로 설정
    bool mbAirFriction;      // 공기 마찰력 사용여부
};
