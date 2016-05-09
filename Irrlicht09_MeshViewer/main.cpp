#include <irrlicht.h>
#include "driverChoice.h"
#include "TutCameraManager.h"
#include "TutGuiManager.h"
#include "TutGuiConstants.h"
#include "TutSceneManager.h"
#include "TutEventReceiver.h"

using namespace irr;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

IrrlichtDevice *Device = 0;
core::stringc StartUpModelFile;
core::stringw MessageText;
core::stringw Caption;
scene::ISceneNode* Model = 0;
scene::ISceneNode* SkyBox = 0;
bool Octree = false;
bool UseLight = false;

scene::ICameraSceneNode* Camera[2] = { 0, 0 };

TutCameraManager* cameraMgr = new TutCameraManager();
TutGuiManager* guiMgr = new TutGuiManager();
TutSceneManager* sceneMgr = new TutSceneManager();
TutEventReceiver* evtRcvr = new	TutEventReceiver();


//Most of the hard work is done.We only need to create the Irrlicht Engine device and all the buttons, menus and toolbars.
//We start up the engine as usual, using createDevice().To make our application catch events, we set our eventreceiver as parameter.
//As you can see, there is also a call to IrrlichtDevice::setResizeable().This makes the render window resizeable, which is quite useful for a mesh viewer.
int main(int argc, char* argv[])
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	if (driverType == video::EDT_COUNT)
		return 1;

	// create device and exit if creation failed
	TutEventReceiver receiver;
	Device = createDevice(driverType, core::dimension2d<u32>(800, 600),
		16, false, false, false, &receiver);

	if (Device == 0)
		return 1; // could not create selected driver.

	Device->setResizable(true);
	Device->setWindowCaption(L"Irrlicht Engine - Loading...");

	video::IVideoDriver* driver = Device->getVideoDriver();
	IGUIEnvironment* env = Device->getGUIEnvironment();
	scene::ISceneManager* smgr = Device->getSceneManager();
	smgr->getParameters()->setAttribute(scene::COLLADA_CREATE_SCENE_INSTANCES, true);

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	smgr->addLightSceneNode(0, core::vector3df(200, 200, 200),
		video::SColorf(1.0f, 1.0f, 1.0f), 2000);
	smgr->setAmbientLight(video::SColorf(0.3f, 0.3f, 0.3f));
	// add our media directory as "search path"
	Device->getFileSystem()->addFileArchive("../../irrmedia/");


	//=== The next step is to read the XML configuration file
	//We need the data stored in there to be written into the global variables StartUpModelFile, MessageText and Caption.This is now done using the Irrlicht Engine integrated XML parser :
	io::IXMLReader* xml = Device->getFileSystem()->createXMLReader(L"config.xml");

	//if the model file was supplied as a command line parameter, then override what was read from the configuration file
	stringc StartUpModelFileCommandLineParameter = "";
	if (argc > 1)
		StartUpModelFileCommandLineParameter = argv[1];
	sceneMgr->GetGlobalModelFileInfo(xml, Caption, MessageText, StartUpModelFileCommandLineParameter);
	
	guiMgr->setFont(env, "fonthaettenschweiler.bmp"); // set a nicer font
	IGUIContextMenu* menu =  guiMgr->addMenu(env); // create menu
	IGUIToolBar* bar = guiMgr->addToolBar(env); // create toolbar
	IGUIComboBox* box = guiMgr->addTextureFilterComboBoxToToolbar(env, bar); // create a combobox for texture filters in the toolbar
	guiMgr->removeSkinTransparency(env); // remove skin transparency (Set alpha to max value)
	guiMgr->createToolBox(Device, Model); // add a tabcontrol
	
	IGUIStaticText* fpstext = env->addStaticText(L"",core::rect<s32>(400, 4, 570, 23), true, false, bar); // create fps text
	IGUIStaticText* postext = env->addStaticText(L"", core::rect<s32>(10, 50, 470, 80), false, false, 0, GUI_ID_POSITION_TEXT); //create position text
	postext->setVisible(false);
	
	// set window caption
	Caption += " - [";
	Caption += driver->getName();
	Caption += "]";
	Device->setWindowCaption(Caption.c_str());

	
	// show about message box and load default model
	if (argc == 1)
		guiMgr->showAboutText(Device, Caption, MessageText);
	sceneMgr->loadModel(StartUpModelFile.c_str());
	// we need to uncheck the menu entries. would be cool to fake a menu event, but
	// that's not so simple. so we do it brute force
	if (menu)
		for (int item = 1; item < 6; ++item)
			menu->setItemChecked(item, false);
	guiMgr->updateScaleInfo(Device, Model);

	// add skybox

	SkyBox = smgr->addSkyBoxSceneNode(
		driver->getTexture("irrlicht2_up.jpg"),
		driver->getTexture("irrlicht2_dn.jpg"),
		driver->getTexture("irrlicht2_lf.jpg"),
		driver->getTexture("irrlicht2_rt.jpg"),
		driver->getTexture("irrlicht2_ft.jpg"),
		driver->getTexture("irrlicht2_bk.jpg"));

	// add a camera scene node
	Camera[0] = smgr->addCameraSceneNodeMaya();
	Camera[0]->setFarValue(20000.f);
	// Maya cameras reposition themselves relative to their target, so target the location
	// where the mesh scene node is placed.
	Camera[0]->setTarget(core::vector3df(0, 30, 0));

	Camera[1] = smgr->addCameraSceneNodeFPS();
	Camera[1]->setFarValue(20000.f);
	Camera[1]->setPosition(core::vector3df(0, 0, -70));
	Camera[1]->setTarget(core::vector3df(0, 30, 0));

	cameraMgr->setActiveCamera(Device, Camera[0]);

	// load the irrlicht engine logo
	IGUIImage *img = guiMgr->addIrrlichtLogo(env);

	// remember state so we notice when the window does lose the focus
	bool hasFocus = Device->isWindowFocused();

	// draw everything
	while (Device->run() && driver)
	{
		// Catch focus changes (workaround until Irrlicht has events for this)
		bool focused = Device->isWindowFocused();
		if (hasFocus && !focused)
			evtRcvr->OnKillFocus();
		hasFocus = focused;

		if (Device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(150, 50, 50, 50));

			smgr->drawAll();
			env->drawAll();

			driver->endScene();

			// update information about current frame-rate
			core::stringw str(L"FPS: ");
			str.append(core::stringw(driver->getFPS()));
			str += L" Tris: ";
			str.append(core::stringw(driver->getPrimitiveCountDrawn()));
			fpstext->setText(str.c_str());

			// update information about the active camera
			scene::ICameraSceneNode* cam = Device->getSceneManager()->getActiveCamera();
			str = L"Pos: ";
			str.append(core::stringw(cam->getPosition().X));
			str += L" ";
			str.append(core::stringw(cam->getPosition().Y));
			str += L" ";
			str.append(core::stringw(cam->getPosition().Z));
			str += L" Tgt: ";
			str.append(core::stringw(cam->getTarget().X));
			str += L" ";
			str.append(core::stringw(cam->getTarget().Y));
			str += L" ";
			str.append(core::stringw(cam->getTarget().Z));
			postext->setText(str.c_str());

			// update the tool dialog
			guiMgr->updateToolBox(Device, Model);
		}
		else
			Device->yield();
	}

	Device->drop();
	return 0;
}
