#pragma once

#include "ACommonIncludes.h"

class ACemara
{
public:
	ACemara();
	~ACemara();

	Matrix	GetViewMatrix() const;
	Vector3 GetPosition() const;
	void	SetPosition(float x, float y, float z);
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




};

