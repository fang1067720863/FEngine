#pragma once

class FRhiSceneObject;
class FTimer;
class FRenderer {
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	//virtual void PreInitialize(double width, double height) = 0;
	virtual void OnResize() = 0;
	//virtual void Draw(FRhiSceneObject *sceneObj) = 0;

	
protected:
private:

};