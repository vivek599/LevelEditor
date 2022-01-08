#include "ACemara.h"

ACemara::ACemara() :
	m_Up(Vector3(0.0f, 1.0f, 0.0f))
{

}

ACemara::~ACemara()
{

}

void ACemara::CreateFrustum(const Matrix& Projection)
{
	m_ViewMatrix = XMMatrixLookAtLH(m_Eye, m_LookAt, m_Up);
	BoundingFrustum::CreateFromMatrix(m_Frustum, m_ViewMatrix * Projection);
}

Matrix ACemara::GetViewMatrix() const
{
	return m_ViewMatrix;
}

void ACemara::SetPosition(float x, float y, float z)
{
	m_Eye = Vector3(x, y, z);
}

void ACemara::SetPosition(Vector3 Position)
{
	m_Eye = Position;
}

void ACemara::SetFocus(float x, float y, float z)
{
	m_LookAt = Vector3(x, y, z);
}

void ACemara::SetFocus(Vector3 Position)
{
	m_LookAt = Position;
}

bool ACemara::Update(float deltaTime)
{
	m_LookAt = Vector3(256.0f, 0.0f, 256.0f);
	m_ViewMatrix = XMMatrixLookAtLH(m_Eye, m_LookAt, m_Up);

	return true;
}

bool ACemara::Render()
{

	return true;
}

Vector3 ACemara::GetPosition() const
{
	return m_Eye;
}

