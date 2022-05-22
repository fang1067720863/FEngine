#pragma once

#include<FPtr.h>
#include<FReference.h>
#include<string>

class FCamera;

struct Traits : public FReference
{
    Traits() {}

    // graphics context original and size
    int x;
    int y;
    int width;
    int height;


    // window decoration and behaviour
    std::string windowName;
    bool        windowDecoration;

    // buffer depths, 0 equals off.
    unsigned int red;
    unsigned int blue;
    unsigned int green;
    unsigned int alpha;
    unsigned int depth;
    unsigned int stencil;

    // multi sample parameters
    unsigned int sampleBuffers;
    unsigned int samples;

    // buffer configuration
    bool pbuffer;
    bool quadBufferStereo;
    bool doubleBuffer;

    unsigned int    level;
    unsigned int    face;
    unsigned int    mipMapGeneration;

    // V-sync
    bool            vsync;

    // Swap Group
    bool            swapGroupEnabled;


    // use multithreaded OpenGL-engine (OS X only)
    bool            useMultiThreadedOpenGLEngine;

    // enable cursor
    bool            useCursor;

};

class FGraphicsContext {


	FPtr<FCamera>  camera;
    FPtr<Traits>   traits;


    //GraphicsOperationQueue              _operations;           // 指令集
	//osg::ref_ptr<Operation>             _currentOperation;


    // cpp 17的callback定义和使用
	//ref_ptr<ResizedCallback>            _resizedCallback;
	//ref_ptr<SwapCallback>               _swapCallback;

	//Timer_t                             _lastClearTick;
	//Vec4                                _clearColor;
	//GLbitfield                          _clearMask;




};