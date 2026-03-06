#include "Time.h"

Time Time::m_instance;

Time& Time::GetInstance()
{
	return m_instance;
}

void Time::Update()
{
	QueryPerformanceCounter(&m_curTick);
	m_time = (m_curTick.QuadPart - m_startTick.QuadPart) / (double)m_tickPerSecond.QuadPart;
	m_deltaTime = (m_curTick.QuadPart - m_lastTick.QuadPart) / (double)m_tickPerSecond.QuadPart;
	m_lastTick = m_curTick;
}

double Time::deltaTime()
{
	return m_deltaTime;
}

double Time::time()
{
	return m_time;
}

Time::Time()
{
	QueryPerformanceFrequency(&m_tickPerSecond);
	QueryPerformanceCounter(&m_startTick);
	m_curTick.QuadPart = m_startTick.QuadPart;
	m_lastTick.QuadPart = m_startTick.QuadPart - 1;	// 计算第一帧时，防止除零错误
	Update();
}

