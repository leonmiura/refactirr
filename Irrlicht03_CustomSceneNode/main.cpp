#include <irrlicht.h>
#include "CSampleSceneNode.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace std;
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

int main()
{

	IrrlichtDevice* device = createDevice(EDT_OPENGL,dimension2d<u32>(640,480));

	if (device == 0) //(device == NULL)
		return 1; //could not create the selected driver.

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");

	smgr->addCameraSceneNode(0,vector3df(0,-40,0),vector3df(0,0,0));

	//using the custom scene node:
	CSampleSceneNode* myNode = new CSampleSceneNode(smgr->getRootSceneNode(), smgr, 666);

	//animating the scene node for fun
	ISceneNodeAnimator* anim =
		smgr->createRotationAnimator(core::vector3df(0.8f, 0.0f, 0.8f));

	if (anim)
	{
		myNode->addAnimator(anim);
		anim->drop();
		anim = 0;
	}

	myNode->drop();
	myNode = 0; // As I shouldn't refer to it again, ensure that I can't

	u32 frames = 0;
	while (device->run())
	{
		driver->beginScene(true, true, video::SColor(0, 100, 100, 100));

		smgr->drawAll();

		driver->endScene();
		if (++frames == 100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}


	device->drop();
	return 0;
}