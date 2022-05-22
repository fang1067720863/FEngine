#pragma once

class FEvent;
class FGUIEvent;
class FEventHandler
{
  public:
	bool virtual handle(const FEvent& event) { return false; }
};

class FGUIEventHandler : public FEventHandler
{
public:
	bool virtual handle(const FGUIEvent& event) { return false; }
};

