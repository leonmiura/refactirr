#include "TutGuiConstants.h"
#include "TutGuiManager.h"
#include "TutSceneManager.h"
using namespace irr;
using namespace io;
using namespace core;
using namespace scene;

extern TutGuiManager* guiMgr;
extern IrrlichtDevice* Device;
extern ISceneNode* Model;
extern stringw Caption;
extern core::stringc StartUpModelFile;
extern bool Octree;
extern bool UseLight;

#pragma region "Function loadModel() "
/// <summary>
/// Loads a model and displays it using an addAnimatedMeshSceneNode and the scene manager. It also displays a short message box, if the model could not be loaded.
/// </summary>
/// <param name="fn">The model's filename.</param>
/// <param name="device">The device.</param>
/// <param name="model">The model.</param>
/// <param name="caption">The caption.</param>
/// <param name="startupModelFile">The startup model file.</param>
/// <param name="octree">if set to <c>true</c> [octree].</param>
/// <param name="useLight">if set to <c>true</c> [use light].</param>
void TutSceneManager::loadModel(const c8* fn)
{
	// modify the name if it a .pk3 file

	io::path filename(fn);

	io::path extension;
	core::getFileNameExtension(extension, filename);
	extension.make_lower();

	// if a texture is loaded apply it to the current model..
	if (extension == ".jpg" || extension == ".pcx" ||
		extension == ".png" || extension == ".ppm" ||
		extension == ".pgm" || extension == ".pbm" ||
		extension == ".psd" || extension == ".tga" ||
		extension == ".bmp" || extension == ".wal" ||
		extension == ".rgb" || extension == ".rgba")
	{
		video::ITexture * texture =
			Device->getVideoDriver()->getTexture(filename);
		if (texture && Model)
		{
			// always reload texture
			Device->getVideoDriver()->removeTexture(texture);
			texture = Device->getVideoDriver()->getTexture(filename);

			Model->setMaterialTexture(0, texture);
		}
		return;
	}
	// if a archive is loaded add it to the FileArchive..
	else if (extension == ".pk3" || extension == ".zip" || extension == ".pak" || extension == ".npk")
	{
		Device->getFileSystem()->addFileArchive(filename.c_str());
		return;
	}

	// load a model into the engine

	if (Model)
		Model->remove();

	Model = 0;

	if (extension == ".irr")
	{
		core::array<scene::ISceneNode*> outNodes;
		Device->getSceneManager()->loadScene(filename);
		Device->getSceneManager()->getSceneNodesFromType(scene::ESNT_ANIMATED_MESH, outNodes);
		if (outNodes.size())
			Model = outNodes[0];
		return;
	}

	scene::IAnimatedMesh* m = Device->getSceneManager()->getMesh(filename.c_str());

	if (!m)
	{
		// model could not be loaded

		if (StartUpModelFile != filename)
			Device->getGUIEnvironment()->addMessageBox(
			Caption.c_str(), L"The model could not be loaded. " \
			L"Maybe it is not a supported file format.");
		return;
	}

	// set default material properties

	if (Octree)
		Model = Device->getSceneManager()->addOctreeSceneNode(m->getMesh(0));
	else
	{
		scene::IAnimatedMeshSceneNode* animModel = Device->getSceneManager()->addAnimatedMeshSceneNode(m);
		animModel->setAnimationSpeed(30);
		Model = animModel;
	}
	Model->setMaterialFlag(video::EMF_LIGHTING, UseLight);
	Model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, UseLight);
	//  Model->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	Model->setDebugDataVisible(scene::EDS_OFF);

	//// we need to uncheck the menu entries. would be cool to fake a menu event, but
	//// that's not so simple. so we do it brute force
	gui::IGUIContextMenu* menu = (gui::IGUIContextMenu*)Device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_TOGGLE_DEBUG_INFO, true);
	if (menu)
		for (int item = 1; item < 6; ++item)
			menu->setItemChecked(item, false);
	guiMgr->updateScaleInfo(Device, Model);

}
#pragma endregion



void TutSceneManager::GetGlobalModelFileInfo(IXMLReader* xml, stringw& Caption, stringw& MessageText, stringc StartUpModelFileCommandLineParameter){
	//io::IXMLReader* xml = Device->getFileSystem()->createXMLReader(L"config.xml");

	while (xml && xml->read())
	{
		switch (xml->getNodeType())
		{
		case io::EXN_TEXT:
			// in this xml file, the only text which occurs is the
			// messageText
			MessageText = xml->getNodeData();
			break;
		case io::EXN_ELEMENT:
		{
			if (core::stringw("startUpModel") == xml->getNodeName())
				StartUpModelFile = xml->getAttributeValue(L"file");
			else
				if (core::stringw("messageText") == xml->getNodeName())
					Caption = xml->getAttributeValue(L"caption");
		}
		break;
		default:
			break;
		}
	}

	if (xml)
		xml->drop(); // don't forget to delete the xml reader

	//if the model file was supplied as a command line parameter, then override what was read from the configuration file
	if (StartUpModelFileCommandLineParameter != "")
		StartUpModelFile = StartUpModelFileCommandLineParameter;
}


