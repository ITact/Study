#include "CTimer.h"

//  默认构造函数，没有指定fps（每秒帧数）
CTimer::CTimer(): m_FPS(0),
				          m_TimeElapsed(0.0f),
				          m_FrameTime(0),
				          m_LastTime(0),
				          m_PerfCountFreq(0)
{
	QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
	
	m_TimeScale = 1.0f/m_PerfCountFreq;
}


//	指定一个fps
CTimer::CTimer(float fps): m_FPS(fps), m_TimeElapsed(0.0f), m_LastTime(0), m_PerfCountFreq(0)
{

	QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);

	m_TimeScale = 1.0f/m_PerfCountFreq;

	m_FrameTime = (LONGLONG)(m_PerfCountFreq / m_FPS);
}



//	调用这个函数开始游戏循环
void CTimer::Start()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &m_LastTime);

	m_NextTime = m_LastTime + m_FrameTime;

	return;
}

bool CTimer::ReadyForNextFrame()
{
	if (!m_FPS)
  {
    MessageBox(NULL, "No FPS set in timer", "Doh!", 0);

    return false;
  }
  
  QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);

	if (m_CurrentTime > m_NextTime)
	{

		m_TimeElapsed	= (m_CurrentTime - m_LastTime) * m_TimeScale;
		m_LastTime		= m_CurrentTime;

		m_NextTime = m_CurrentTime + m_FrameTime;

		return true;
	}

	return false;
}

double CTimer::TimeElapsed()
{
	QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
	
	m_TimeElapsed	= (m_CurrentTime - m_LastTime) * m_TimeScale;
	
	m_LastTime		= m_CurrentTime;

	return m_TimeElapsed;
		
}
