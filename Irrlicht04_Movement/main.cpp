#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;



class MyEventReceiver : public IEventReceiver
{
public: 
	virtual bool OnEvent(const SEvent& event)
	{
		//Remember whether each key is up or down
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

			return false; //why?
	}

	//This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i) //again, why ++i?
			KeyIsDown[i] = false;
	}

private: 
	bool KeyIsDown[KEY_KEY_CODES_COUNT]; //we use this array to store the current state of each key
};


int main()
{
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	// create device
	MyEventReceiver receiver;
	IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);
	if (device == 0)
		return 1; //could not create OpenGL driver

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	//create sphere scene node
	scene::ISceneNode* node = smgr->addSphereSceneNode();
	if (node)
	{
		node->setPosition(core::vector3df(0, 0, 30));
		node->setMaterialTexture(0, driver->getTexture("../../irrmedia/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING,false);
	}


	//create cube scene node and make it fly around the sphere
	scene::ISceneNode* n = smgr->addCubeSceneNode();

	if (n)
	{
		n->setMaterialTexture(0, driver->getTexture("../../irrmedia/t351sml.jpg"));
		n->setMaterialFlag(video::EMF_LIGHTING, false); //glDisable(GL_LIGHTING)
		scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(core::vector3df(0, 0, 30), 20.0f);
		if (anim)
		{
			n->addAnimator(anim);
			anim->drop();
		}
	}

	//create ninja scene node, which will run around the scene
	scene::IAnimatedMeshSceneNode* anms = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../irrmedia/ninja.b3d"));

	if (anms)
	{
		scene::ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(core::vector3df(100,0,60),core::vector3df(-100,0,60),3500,true);
		if (anim)
		{
			anms->addAnimator(anim);
			anim->drop();
		}

		//see note about animation constants and not hard-coding frame numbers
		anms->setMaterialFlag(video::EMF_LIGHTING, false);

		anms->setFrameLoop(0, 13);
		anms->setAnimationSpeed(15);
		//anms->setMD2Animation(scene::EMAT_RUN); //this I get

		anms->setScale(core::vector3df(2.f, 2.f, 2.f));
		anms->setRotation(core::vector3df(0, -90, 0));
		//anms->setMaterialTexture(0, driver->getTexture("../../irrmedia/sydney.bmp")); //this I don't

		smgr->addCameraSceneNodeFPS();
		device->getCursorControl()->setVisible(false);

		device->getGUIEnvironment()->addImage(driver->getTexture("../../irrmedia/irrlichtlogoalpha2.tga"),core::position2d<s32>(10,20));

		gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"",core::rect<s32>(10,10,400,20));
		diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));
	}


	//rendering:
	int lastFPS = -1;

	//In order to do framerate independent movement, we have to know how long it's been since the last frame
	u32 then = device->getTimer()->getTime();

	//This is the movement speed in units per second
	const f32 MOVEMENT_SPEED = 5.f;

	while (device->run())
	{
		//Work out a frame delta time
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; //time in seconds
		then = now;


		//check if keys are being held down and move the sphere around accordingly
		core::vector3df nodePosition = node->getPosition();

		if (receiver.IsKeyDown(irr::KEY_KEY_W))
			nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		else if (receiver.IsKeyDown(irr::KEY_KEY_S))
			nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

		if (receiver.IsKeyDown(irr::KEY_KEY_A))
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		else if (receiver.IsKeyDown(irr::KEY_KEY_D))
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

		node->setPosition(nodePosition);

		driver->beginScene(true, true, video::SColor(255, 113, 113, 133));

		smgr->drawAll(); //draw the 3d scene
		device->getGUIEnvironment()->drawAll(); //draw the GUI environment (the Irrlicht logo)

		driver->endScene();

		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}