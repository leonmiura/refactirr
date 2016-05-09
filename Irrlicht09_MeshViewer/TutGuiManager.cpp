#include <irrlicht.h>
#include "TutGuiManager.h"
#include "TutGuiConstants.h";

IGUIContextMenu* TutGuiManager::addMenu(IGUIEnvironment* env)
{
	IGUIContextMenu* menu = env->addMenu();
	menu->addItem(L"File", -1, true, true);
	menu->addItem(L"View", -1, true, true);
	menu->addItem(L"Camera", -1, true, true);
	menu->addItem(L"Help", -1, true, true);

	IGUIContextMenu* submenu;
	submenu = menu->getSubMenu(0);
	submenu->addItem(L"Open Model File & Texture...", GUI_ID_OPEN_MODEL);
	submenu->addItem(L"Set Model Archive...", GUI_ID_SET_MODEL_ARCHIVE);
	submenu->addItem(L"Load as Octree", GUI_ID_LOAD_AS_OCTREE);
	submenu->addSeparator();
	submenu->addItem(L"Quit", GUI_ID_QUIT);

	submenu = menu->getSubMenu(1);
	submenu->addItem(L"sky box visible", GUI_ID_SKY_BOX_VISIBLE, true, false, true);
	submenu->addItem(L"toggle model debug information", GUI_ID_TOGGLE_DEBUG_INFO, true, true);
	submenu->addItem(L"model material", -1, true, true);

	submenu = submenu->getSubMenu(1);
	submenu->addItem(L"Off", GUI_ID_DEBUG_OFF);
	submenu->addItem(L"Bounding Box", GUI_ID_DEBUG_BOUNDING_BOX);
	submenu->addItem(L"Normals", GUI_ID_DEBUG_NORMALS);
	submenu->addItem(L"Skeleton", GUI_ID_DEBUG_SKELETON);
	submenu->addItem(L"Wire overlay", GUI_ID_DEBUG_WIRE_OVERLAY);
	submenu->addItem(L"Half-Transparent", GUI_ID_DEBUG_HALF_TRANSPARENT);
	submenu->addItem(L"Buffers bounding boxes", GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES);
	submenu->addItem(L"All", GUI_ID_DEBUG_ALL);

	submenu = menu->getSubMenu(1)->getSubMenu(2);
	submenu->addItem(L"Solid", GUI_ID_MODEL_MATERIAL_SOLID);
	submenu->addItem(L"Transparent", GUI_ID_MODEL_MATERIAL_TRANSPARENT);
	submenu->addItem(L"Reflection", GUI_ID_MODEL_MATERIAL_REFLECTION);

	submenu = menu->getSubMenu(2);
	submenu->addItem(L"Maya Style", GUI_ID_CAMERA_MAYA);
	submenu->addItem(L"First Person", GUI_ID_CAMERA_FIRST_PERSON);

	submenu = menu->getSubMenu(3);
	submenu->addItem(L"About", GUI_ID_ABOUT);

	return menu;
}

IGUIToolBar* TutGuiManager::addToolBar(IGUIEnvironment* env)
{
	IVideoDriver* driver = env->getVideoDriver();
	IGUIToolBar* bar = env->addToolBar();
	ITexture* image = driver->getTexture("open.png");
	bar->addButton(GUI_ID_BUTTON_OPEN_MODEL, 0, L"Open a model", image, 0, false, true);

	image = driver->getTexture("tools.png");
	bar->addButton(GUI_ID_BUTTON_SHOW_TOOLBOX, 0, L"Open Toolset", image, 0, false, true);

	image = driver->getTexture("zip.png");
	bar->addButton(GUI_ID_BUTTON_SELECT_ARCHIVE, 0, L"Set Model Archive", image, 0, false, true);

	image = driver->getTexture("help.png");
	bar->addButton(GUI_ID_BUTTON_SHOW_ABOUT, 0, L"Open Help", image, 0, false, true);

	return bar;
}

IGUIComboBox* TutGuiManager::addTextureFilterComboBoxToToolbar(IGUIEnvironment* env, IGUIToolBar* bar)
{
	gui::IGUIComboBox* box = env->addComboBox(core::rect<s32>(250, 4, 350, 23), bar, GUI_ID_TEXTUREFILTER);
	box->addItem(L"No filtering");
	box->addItem(L"Bilinear");
	box->addItem(L"Trilinear");
	box->addItem(L"Anisotropic");
	box->addItem(L"Isotropic");

	return box;
}

void TutGuiManager::removeSkinTransparency(IGUIEnvironment* env)
{
	for (s32 i = 0; i<gui::EGDC_COUNT; ++i)
	{
		video::SColor col = env->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
		col.setAlpha(255);
		env->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
	}
}



#pragma region "Function createToolBox() creates a toolbox window. In this simple mesh viewer, this toolbox only contains a tab control with three edit boxes for changing the scale of the displayed model."
void TutGuiManager::createToolBox(IrrlichtDevice* device, ISceneNode* model)
{
	// remove tool box if already there
	IGUIEnvironment* env = device->getGUIEnvironment();
	IGUIElement* root = env->getRootGUIElement();
	IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (e)
		e->remove();

	// create the toolbox window
	IGUIWindow* wnd = env->addWindow(core::rect<s32>(600, 45, 800, 480),
		false, L"Toolset", 0, GUI_ID_DIALOG_ROOT_WINDOW);

	// create tab control and tabs
	IGUITabControl* tab = env->addTabControl(
		core::rect<s32>(2, 20, 800 - 602, 480 - 7), wnd, true, true);

	IGUITab* t1 = tab->addTab(L"Config");

	// add some edit boxes and a button to tab one
	env->addStaticText(L"Scale:",
		core::rect<s32>(10, 20, 60, 45), false, false, t1);
	env->addStaticText(L"X:", core::rect<s32>(22, 48, 40, 66), false, false, t1);
	env->addEditBox(L"1.0", core::rect<s32>(40, 46, 130, 66), true, t1, GUI_ID_X_SCALE);
	env->addStaticText(L"Y:", core::rect<s32>(22, 82, 40, 96), false, false, t1);
	env->addEditBox(L"1.0", core::rect<s32>(40, 76, 130, 96), true, t1, GUI_ID_Y_SCALE);
	env->addStaticText(L"Z:", core::rect<s32>(22, 108, 40, 126), false, false, t1);
	env->addEditBox(L"1.0", core::rect<s32>(40, 106, 130, 126), true, t1, GUI_ID_Z_SCALE);

	env->addButton(core::rect<s32>(10, 134, 85, 165), t1, GUI_ID_BUTTON_SET_SCALE, L"Set");

	// quick scale buttons
	env->addButton(core::rect<s32>(65, 20, 95, 40), t1, GUI_ID_BUTTON_SCALE_MUL10, L"* 10");
	env->addButton(core::rect<s32>(100, 20, 130, 40), t1, GUI_ID_BUTTON_SCALE_DIV10, L"* 0.1");

	updateScaleInfo(device, model);

	// add transparency control
	env->addStaticText(L"GUI Transparency Control:",
		core::rect<s32>(10, 200, 150, 225), true, false, t1);
	IGUIScrollBar* scrollbar = env->addScrollBar(true,
		core::rect<s32>(10, 225, 150, 240), t1, GUI_ID_SKIN_TRANSPARENCY);
	scrollbar->setMax(255);
	scrollbar->setPos(255);

	// add framerate control
	env->addStaticText(L":", core::rect<s32>(10, 240, 150, 265), true, false, t1);
	env->addStaticText(L"Framerate:",
		core::rect<s32>(12, 240, 75, 265), false, false, t1);
	// current frame info
	env->addStaticText(L"", core::rect<s32>(75, 240, 200, 265), false, false, t1,
		GUI_ID_ANIMATION_INFO);
	scrollbar = env->addScrollBar(true,
		core::rect<s32>(10, 265, 150, 280), t1, GUI_ID_SKIN_ANIMATION_FPS);
	scrollbar->setMax(MAX_FRAMERATE);
	scrollbar->setMin(-(MAX_FRAMERATE));
	scrollbar->setPos(DEFAULT_FRAMERATE);
	scrollbar->setSmallStep(1);
}
#pragma endregion

#pragma region "Function updateToolBox() is called each frame to update dynamic information in the toolbox."
void TutGuiManager::updateToolBox(IrrlichtDevice* device, ISceneNode* model)
{
	IGUIEnvironment* env = device->getGUIEnvironment();
	IGUIElement* root = env->getRootGUIElement();
	IGUIElement* dlg = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (!dlg)
		return;
	
	// update the info we have about the animation of the model
	IGUIStaticText *  aniInfo = (IGUIStaticText *)(dlg->getElementFromId(GUI_ID_ANIMATION_INFO, true));
	if (aniInfo)
	{
		if (model && scene::ESNT_ANIMATED_MESH == model->getType())
		{
			scene::IAnimatedMeshSceneNode* animatedModel = (scene::IAnimatedMeshSceneNode*)model;

			core::stringw str((s32)core::round_(animatedModel->getAnimationSpeed()));
			str += L" Frame: ";
			str += core::stringw((s32)animatedModel->getFrameNr());
			aniInfo->setText(str.c_str());
		}
		else
			aniInfo->setText(L"");
	}
}
#pragma endregion


#pragma region "Update the display of the model scaling"
void TutGuiManager::updateScaleInfo(IrrlichtDevice* device, ISceneNode* model)
{
	IGUIElement* toolboxWnd = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (!toolboxWnd)
		return;
	if (!model)
	{
		toolboxWnd->getElementFromId(GUI_ID_X_SCALE, true)->setText(L"-");
		toolboxWnd->getElementFromId(GUI_ID_Y_SCALE, true)->setText(L"-");
		toolboxWnd->getElementFromId(GUI_ID_Z_SCALE, true)->setText(L"-");
	}
	else
	{
		core::vector3df scale = model->getScale();
		toolboxWnd->getElementFromId(GUI_ID_X_SCALE, true)->setText(core::stringw(scale.X).c_str());
		toolboxWnd->getElementFromId(GUI_ID_Y_SCALE, true)->setText(core::stringw(scale.Y).c_str());
		toolboxWnd->getElementFromId(GUI_ID_Z_SCALE, true)->setText(core::stringw(scale.Z).c_str());
	}
}
#pragma endregion


#pragma region "Set the skin transparency by changing the alpha values of all skin-colors"
void TutGuiManager::setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
	for (s32 i = 0; i<irr::gui::EGDC_COUNT; ++i)
	{
		video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
}
#pragma endregion


IGUIImage * TutGuiManager::addIrrlichtLogo(IGUIEnvironment* env)
{
	IVideoDriver* driver = env->getVideoDriver();
	IGUIImage* img = env->addImage(driver->getTexture("irrlichtlogo2.png"),
		core::position2d<s32>(10, driver->getScreenSize().Height - 128));

	// lock the logo's edges to the bottom left corner of the screen
	img->setAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT,
		EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT);

	return img;
}



void TutGuiManager::setFont(IGUIEnvironment* env, stringc fontName)
{
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont(fontName);
	if (font)
		skin->setFont(font);
}


#pragma region "Function showAboutText() displays a messagebox with a caption and a message text. The texts will be stored in the MessageText and Caption variables at startup."
void TutGuiManager::showAboutText(IrrlichtDevice* device, core::stringw caption, core::stringw messageText)
{
	// create modal message box with the text
	// loaded from the xml file.
	device->getGUIEnvironment()->addMessageBox(
		caption.c_str(), messageText.c_str());
}
#pragma endregion
