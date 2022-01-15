#include "ADXTime.h"

ADXTime::ADXTime()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_StartingTime);
	QueryPerformanceCounter(&m_EndingTime);
}

ADXTime::~ADXTime()
{
}

void ADXTime::Start()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_StartingTime);
}

float ADXTime::End()
{
	QueryPerformanceCounter(&m_EndingTime);
	m_ElapsedMicroseconds.QuadPart = m_EndingTime.QuadPart - m_StartingTime.QuadPart;
	return GetMSec();
}

float ADXTime::GetSeconds()
{
	return GetMSec() / 1000.0f;
}

float ADXTime::GetMSec()
{
	m_ElapsedMicroseconds.QuadPart *= 1000000;
	m_ElapsedMicroseconds.QuadPart /= m_Frequency.QuadPart;

	float deltaTime = (m_ElapsedMicroseconds.QuadPart) / (1000.f);
	return deltaTime;
}







