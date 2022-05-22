#pragma once


typedef unsigned __int64 Timer_t;

class FTimer {
public:
	FTimer();

	float TotalTime()const;		// ����Ϸʱ��
	float DeltaTime()const;		// ֡���ʱ��

	void Reset();               // ����Ϣѭ��֮ǰ����
	void Start();               // ��ȡ����ͣ��ʱ�����
	void Stop();                // ����ͣ��ʱ�����
	void Tick();                // ��ÿһ֡��ʱ�����

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	Timer_t m_BaseTime;
	Timer_t m_PausedTime;
	Timer_t m_StopTime;
	Timer_t m_PrevTime;
	Timer_t m_CurrTime;

	bool m_Stopped;
};