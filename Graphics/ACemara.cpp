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

void ACemara::SetRotation(float pitch, float yaw, float roll)
{
	m_Rotation = Vector3(pitch, yaw, roll);
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
	m_ViewMatrix = XMMatrixLookAtLH(m_Eye, m_LookAt, m_Up);

	float multiplier = HIWORD(GetAsyncKeyState(VK_LSHIFT)) ? 30.0f : 5.0f;
	const float movementSpeed = deltaTime * 5.0f * multiplier;
	const float rotationSpeed = deltaTime * 0.1f * multiplier;
	POINT point;
	GetCursorPos(&point);
	static POINT oldPoint = point;

	POINT newPoint;
	GetCursorPos(&newPoint);

	auto diffx = newPoint.x - oldPoint.x;
	auto diffy = newPoint.y - oldPoint.y;

	if (HIWORD(GetAsyncKeyState(VK_RBUTTON)))
	{
		m_Rotation.x += diffy * rotationSpeed;
		m_Rotation.y += diffx * rotationSpeed;

		m_Rotation.x = Math::Clamp(m_Rotation.x, -XM_PIDIV2 * 0.9f, XM_PIDIV2 * 0.9f);
	}

	oldPoint = newPoint;

	Matrix RotationMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_Forward = XMVector3TransformCoord(DEFAULT_FORWARD, RotationMatrix);
	m_Right = XMVector3TransformCoord(DEFAULT_RIGHT, RotationMatrix);
	m_Forward.Normalize();
	m_Right.Normalize();

	if (HIWORD(GetAsyncKeyState('A')))
	{
		m_Eye -= m_Right * movementSpeed;
	}

	if (HIWORD(GetAsyncKeyState('D')))
	{
		m_Eye += m_Right * movementSpeed;
	}

	if (HIWORD(GetAsyncKeyState('W')))
	{
		m_Eye += m_Forward * movementSpeed;
	}

	if (HIWORD(GetAsyncKeyState('S')))
	{
		m_Eye -= m_Forward * movementSpeed;
	}

	if (HIWORD(GetAsyncKeyState(VK_SPACE)))
	{
		m_Forward = DEFAULT_FORWARD;
		m_Right = DEFAULT_RIGHT;
		SetRotation(0.0f, 0.0f, 0.0f);
	}

	if (HIWORD(GetAsyncKeyState('Q')))
	{
		m_Eye += m_Up * movementSpeed;
	}

	if (HIWORD(GetAsyncKeyState('E')))
	{
		m_Eye -= m_Up * movementSpeed;
	}

	if (HIWORD(GetAsyncKeyState('L')))
	{
		SetFocus(Vector3(0.0f, 0.0f, 100.0f));
	}

	m_LookAt = m_Eye + m_Forward;

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

