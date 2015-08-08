#include "Camera.h"

void FirstPersonCamera::initialize(const UINT &width, const UINT &height)
{
	mDefFwd = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	mDefRight = XMVectorSet(1.0f, 0, 0.0f, 1.0f);
	mRight = mDefRight;
	mForward = mDefFwd;
	mMoveLR = 0.0f;
	mMoveBF = 0.0f;
	mYaw = 0.0f;
	mPitch = 0.0f;
	mPosition = XMVectorSet(0.0f, 0.5f, -5.0f, 1.0f);
	mTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	mUp = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(mPosition, mTarget, mUp));
	XMStoreFloat4x4(&mProjection, XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)DirectX11Core::mWindowWidth / (float)DirectX11Core::mWindowHeight, 0.1f, 1000.0f));

	mCBData.viewProj = XMMatrixTranspose(XMLoadFloat4x4(&mView) * XMLoadFloat4x4(&mProjection));
	mCBuffer = DirectX11Core::createConstantBuffer(sizeof(CBCamera), 1, 0, &mCBData);

	ID3D11Buffer *cbuffers[] = { mCBuffer };
	DirectX11Core::mDeviceContext->VSSetConstantBuffers(0, 1, cbuffers);
}

void FirstPersonCamera::update(const float &lr, const float &bf, const float &yaw, const float &pitch)
{
	mMoveBF = bf;
	mMoveLR = lr;
	mYaw = yaw;
	mPitch = pitch;

	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0);
	mTarget = XMVector3Normalize(XMVector3TransformCoord(mDefFwd, rotMat));

	/*
	// FPS camera
	XMMATRIX RotateYTempMatrix = XMMatrixRotationY(mYaw);
	mRight = XMVector3TransformCoord(mDefRight, RotateYTempMatrix);
	mUp = XMVector3TransformCoord(mUp, RotateYTempMatrix);
	mForward = XMVector3TransformCoord(mDefFwd, RotateYTempMatrix);
	*/

	// Free-Look Camera
	mRight = XMVector3TransformCoord(mDefRight, rotMat);
	mForward = XMVector3TransformCoord(mDefFwd, rotMat);
	mUp = XMVector3Cross(mForward, mRight);

	mPosition += mMoveLR*mRight;
	mPosition += mMoveBF*mForward;
	mMoveLR = 0.0f;
	mMoveBF = 0.0f;
	mTarget += mPosition;
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(mPosition, mTarget, mUp));

	mCBData.viewProj = XMMatrixTranspose(XMLoadFloat4x4(&mView) * XMLoadFloat4x4(&mProjection));
	DirectX11Core::mDeviceContext->UpdateSubresource(mCBuffer, 0, 0, &mCBData, 0, 0);
}