#pragma once
#ifndef _SY_TIME_H
#define _SY_TIME_H
#include <Windows.h>
/* 时间类，记录每帧时间以及程序运行以来的时间 */
class Time
{
private:
	static Time m_instance;
public:
	static Time& GetInstance();

private:
	double m_deltaTime;
	double m_time;

	LARGE_INTEGER m_tickPerSecond;
	LARGE_INTEGER m_curTick;
	LARGE_INTEGER m_lastTick;
	LARGE_INTEGER m_startTick;
public:
	void Update();			// 每帧更新时间
	double deltaTime();		// 据上一帧到当前帧之间的时间，单位 s
	double time();			// 程序自开始以来已经运行的时间，单位 s
private:
	Time();
	Time(const Time& t) = delete;
	Time& operator=(const Time& t) = delete;
};
#endif // !_SY_TIME_H

