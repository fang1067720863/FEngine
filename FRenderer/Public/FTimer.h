#pragma once




class FTimer {
public:
	typedef unsigned __int64 Timer_t;

	FTimer();

	float TotalTime()const;		// 总游戏时间
	float DeltaTime()const;		// 帧间隔时间

	void Reset();               // 在消息循环之前调用
	void Start();               // 在取消暂停的时候调用
	void Stop();                // 在暂停的时候调用
	void Tick();                // 在每一帧的时候调用

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