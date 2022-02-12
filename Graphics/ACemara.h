#pragma once

#include "ACommonIncludes.h"

class ACemara
{
public:
	ACemara();
	~ACemara();
	void CreateFrustum(const Matrix& Projection);
	Matrix	GetViewMatrix() const;
	Vector3 GetPosition() const;
	void	SetPosition(float x, float y, float z);
	void	SetRotation(float pitch, float yaw, float roll);
	void	SetPosition(Vector3 Position);
	void	SetFocus(float x, float y, float z);
	void	SetFocus(Vector3 Position);

	bool Update(float deltaTime);
	bool Render();



private:

	Matrix			m_ViewMatrix;
	Vector3			m_Rotation;
	Vector3			m_Eye;
	Vector3			m_LookAt;
	Vector3			m_Up;
	BoundingFrustum m_Frustum;

	const Vector3 DEFAULT_UP = Vector3(0.0f, 1.0f, 0.0f);
	const Vector3 DEFAULT_FORWARD = Vector3(0.0f, 0.0f, 1.0f);
	const Vector3 DEFAULT_RIGHT = Vector3(1.0f, 0.0f, 0.0f);

	Vector3                                 m_Forward;
	Vector3                                 m_Right;

};

