#pragma once

#include"Ptr.h"
#include"Scene.h"
#include"FGraphicWindowWin32.h"
#include"FDx11Renderer.h"
#include"FTimer.h"

class FView
{
public:

protected:
	FTimer                            m_Timer;

	//Devices                                 _eventSources;

	//osg::ref_ptr<osgViewer::Scene>          _scene;
	//osg::ref_ptr<osgGA::EventQueue>         _eventQueue;
	//osg::ref_ptr<osgGA::CameraManipulator>  _cameraManipulator;
	//EventHandlers                           _eventHandlers;
};


class FCore: public FView
{
public:
	FCore(HINSTANCE hInstance);
	
	bool Realize();
	int Run();

	FGraphicWindowWin32* CreateEmbbedWindow(int x, int y, int width, int height);
private:

	void Update(float dt);
	void DrawSceneObjects(FRhiSceneObject* sObject);
	void Draw();

	FRenderer* renderer;
	FGraphicWindowWin32* window;
	HINSTANCE m_hAppInst;        // Ó¦ÓÃÊµÀý¾ä±ú
	bool m_AppPaused = false;
};