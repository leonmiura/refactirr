#ifndef __TUT_SCENE_MANAGER
#define __TUT_SCENE_MANAGER

#include <irrlicht.h>

using namespace irr;
using namespace io;
using namespace core;
using namespace scene;

//extern stringc StartupModelFile;

/// <summary>
/// Provides methods to manage the tutorial's scene
/// </summary>
class TutSceneManager
{
public:
	//void loadModel(const c8* fn);
	void loadModel(const c8* fn);
	//void GetModelFileInfoByRef(IXMLReader* xml, stringc& StartUpModelFile2, stringw& Caption, stringw& MessageText, stringc StartUpModelFileCommandLineParameter);
	void GetGlobalModelFileInfo(IXMLReader* xml, stringw& Caption, stringw& MessageText, stringc StartUpModelFileCommandLineParameter);
};

#endif