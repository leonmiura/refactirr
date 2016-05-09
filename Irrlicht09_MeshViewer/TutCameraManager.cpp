#include "TutCameraManager.h"
#include "TutSceneManager.h"
#include "TutGuiManager.h"

//global functions declared and initialized in main.cpp
extern IrrlichtDevice *Device;
//extern core::stringc StartUpModelFile;
extern core::stringw MessageText;
extern core::stringw Caption;
extern scene::ISceneNode* Model;
extern scene::ISceneNode* SkyBox;
extern bool Octree;
extern bool UseLight;
extern scene::ICameraSceneNode* Camera[2];
extern TutCameraManager* cameraMgr;
extern TutGuiManager* guiMgr;
extern TutSceneManager* sceneMgr;


/// <summary>
/// Toggle between various cameras
/// </summary>
/// <param name="device">The device.</param>
/// <param name="newActive">The new active.</param>
void TutCameraManager::setActiveCamera(IrrlichtDevice* device, scene::ICameraSceneNode* newActive)
{
	if (0 == device)
		return;

	scene::ICameraSceneNode * active = device->getSceneManager()->getActiveCamera();
	active->setInputReceiverEnabled(false);

	newActive->setInputReceiverEnabled(true);
	device->getSceneManager()->setActiveCamera(newActive);
}