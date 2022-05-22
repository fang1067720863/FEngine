#pragma once

// interfaceClass
class FCore
{
public:
	virtual void Run() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	virtual void PreInitialize() = 0;
protected:
	bool                                                _firstFrame;
	bool                                                _done;
	int                                                 _keyEventSetsDone;
	bool                                                _quitEventSetsDone;
	int                                                 mFrameCount = 0u;
};

