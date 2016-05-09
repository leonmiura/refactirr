#include <irrlicht.h>
#include <iostream>
#include <driverChoice.h>

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

bool debugMode = true;

int main()
{
	// ask user for driver

	video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	

	// create a NULL device to detect screen resolution
	IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
	core::dimension2d<u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	nulldevice->drop();

	// now the dimensions can be used to create the real device


	IrrlichtDevice* device = createDevice(driverType, deskres);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();	

	//To display the Quake 3 map, we first need to load it. Quake 3 maps are packed into .pk3 files which are nothing else than .zip files. So we add the .pk3 file to our irr::io::IFileSystem. After it was added, we are able to read from the files in that archive as if they are directly stored on the disk.
	device->getFileSystem()->addFileArchive("../../irrmedia/map-20kdm2.pk3");

	//Load the mesh
	/*
	Now we can load the mesh by calling irr::scene::ISceneManager::getMesh(). We get a pointer returned to an irr::scene::IAnimatedMesh. As you might know, Quake 3 maps are not really animated, they are only a huge chunk of static geometry with some materials attached. Hence the IAnimatedMesh consists of only one frame, so we get the "first frame" of the "animation", which is our quake level and create an Octree scene node with it, using irr::scene::ISceneManager::addOctreeSceneNode(). The Octree optimizes the scene a little bit, trying to draw only geometry which is currently visible. An alternative to the Octree would be a irr::scene::IMeshSceneNode, which would always draw the complete geometry of the mesh, without optimization. Try it: Use irr::scene::ISceneManager::addMeshSceneNode() instead of addOctreeSceneNode() and compare the primitives drawn by the video driver. (There is a irr::video::IVideoDriver::getPrimitiveCountDrawn() method in the irr::video::IVideoDriver class). Note that this optimization with the Octree is only useful when drawing huge meshes consisting of lots of geometry.
	*/
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;

	if (mesh)
	{
		//using octree for optimization
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024); // why 1024? tried 0, 1, 500, 1000 and saw no difference
		//non-optimized for comparing
		//node = smgr->addMeshSceneNode(mesh->getMesh(0));
	}

	//add FPS camera. There are other types of camera, e.g.: MayaCamera, which is controlled like a Maya viewport camera.
	smgr->addCameraSceneNodeFPS(NULL,200); //rotate speed: 200. Default is 100
	//smgr->addCameraSceneNodeMaya();


	//game loop:
	int lastFPS = -1;
	while (device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(255, 200, 200, 200)); //backbuffer:true, zbuffer:true, clearcolor: some light gray
			smgr->drawAll();
			driver->endScene();

			if (debugMode)
			{
				int fps = driver->getFPS();
				if (lastFPS != fps)
				{
					core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
					str += driver->getName();
					str += "] FPS: ";
					str += fps;

					device->setWindowCaption(str.c_str());
					lastFPS = fps;
				}
			}
		}
		else
			device->yield();//avoid rendering when window is inactive
	}

	device->drop();
	return 0;
}