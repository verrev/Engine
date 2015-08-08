#pragma once
#include "DirectX11Core.h"

struct CBCamera
{
	XMMATRIX viewProj;
};

class Camera
{
public:
	virtual void initialize(const UINT &width, const UINT &height) = 0;
	virtual void update(const float &lr, const float &bf, const float &yaw, const float &pitch) = 0;
	inline virtual XMFLOAT3 getPos() = 0;
	inline virtual float getYaw() = 0;
	inline virtual float getPitch() = 0;
	inline virtual XMMATRIX getViewMatrix() = 0;
	inline virtual XMFLOAT3 getTarget() = 0;
};

class FirstPersonCamera : public Camera
{
public:
	void initialize(const UINT &width, const UINT &height) override;
	void update(const float &lr, const float &bf, const float &yaw, const float &pitch) override;
	inline XMFLOAT3 getPos() override
	{
		return XMFLOAT3(XMVectorGetX(mPosition), XMVectorGetY(mPosition), XMVectorGetZ(mPosition));
	}
	inline float getYaw() override
	{
		return mYaw;
	}
	inline float getPitch() override
	{
		return mPitch;
	}
	inline XMMATRIX getViewMatrix() override
	{
		return XMLoadFloat4x4(&mView);
	}
	inline XMFLOAT3 getTarget() override
	{
		return XMFLOAT3(XMVectorGetX(mTarget), XMVectorGetY(mTarget), XMVectorGetZ(mTarget));
	}

private:
	ID3D11Buffer *mCBuffer;
	CBCamera mCBData;
	XMFLOAT4X4 mView, mProjection;
	XMVECTOR mPosition, mTarget, mUp;
	XMVECTOR mDefFwd, mDefRight;
	XMVECTOR mRight, mForward;
	float mMoveLR, mMoveBF;
	float mYaw, mPitch;
};