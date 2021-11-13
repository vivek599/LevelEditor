#pragma once


#include <Windows.h>

class ADXTime
{
public:
	ADXTime();
	~ADXTime();
	void Start();
	float End();

private:
	float GetMilliSeconds();
	float GetSeconds();

	LARGE_INTEGER m_StartingTime;
	LARGE_INTEGER m_EndingTime;
	LARGE_INTEGER m_ElapsedMicroseconds;
	LARGE_INTEGER m_Frequency;
};




