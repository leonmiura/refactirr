#ifndef __TUT_CAMERA_MANAGER
#define __TUT_CAMERA_MANAGER

#include <irrlicht.h>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;


/// <summary>
/// Provides methods to control the tutorial's camera
/// </summary>
class TutCameraManager{
public:
	void setActiveCamera(IrrlichtDevice* device, scene::ICameraSceneNode* newActive);
};

#endif