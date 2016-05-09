#include "TutEventReceiver.h"

//global functions declared and initialized in main.cpp
extern IrrlichtDevice *Device;
extern core::stringc StartUpModelFile;
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
/// Checks if we currently have a modal dialog open.
/// </summary>
/// <returns>true if there is an open modal dialog, and false if there isn't.</returns>
bool TutEventReceiver::hasModalDialog()
{
	if (!Device)
		return false;
	IGUIEnvironment* env = Device->getGUIEnvironment();
	IGUIElement * focused = env->getFocus();
	while (focused)
	{
		if (focused->isVisible() && focused->hasType(EGUIET_MODAL_SCREEN))
			return true;
		focused = focused->getParent();
	}
	return false;
}


/// <summary>
/// Handles the event of losing focus (switching to another window).
/// </summary>
void TutEventReceiver::OnKillFocus()
{
	// Avoid that the FPS-camera continues moving when the user presses alt-tab while 
	// moving the camera. 
	const core::list<scene::ISceneNodeAnimator*>& animators = Camera[1]->getAnimators();
	core::list<irr::scene::ISceneNodeAnimator*>::ConstIterator iter = animators.begin();
	while (iter != animators.end())
	{
		if ((*iter)->getType() == scene::ESNAT_CAMERA_FPS)
		{
			// we send a key-down event for all keys used by this animator
			scene::ISceneNodeAnimatorCameraFPS * fpsAnimator = static_cast<scene::ISceneNodeAnimatorCameraFPS*>(*iter);
			const core::array<SKeyMap>& keyMap = fpsAnimator->getKeyMap();
			for (irr::u32 i = 0; i< keyMap.size(); ++i)
			{
				irr::SEvent event;
				event.EventType = EET_KEY_INPUT_EVENT;
				event.KeyInput.Key = keyMap[i].KeyCode;
				event.KeyInput.PressedDown = false;
				fpsAnimator->OnEvent(event);
			}
		}
		++iter;
	}
}


/// <summary>
/// Handles "menu item clicked" events.
/// </summary>
/// <param name="menu">The menu which contains the item that was clicked.</param>
void TutEventReceiver::OnMenuItemSelected(IGUIContextMenu* menu)
{
	s32 id = menu->getItemCommandId(menu->getSelectedItem());
	IGUIEnvironment* env = Device->getGUIEnvironment();

	switch (id)
	{
	case GUI_ID_OPEN_MODEL: // FilOnButtonSetScalinge -> Open Model
		env->addFileOpenDialog(L"Please select a model file to open");
		break;
	case GUI_ID_SET_MODEL_ARCHIVE: // File -> Set Model Archive
		env->addFileOpenDialog(L"Please select your game archive/directory");
		break;
	case GUI_ID_LOAD_AS_OCTREE: // File -> LoadAsOctree
		Octree = !Octree;
		menu->setItemChecked(menu->getSelectedItem(), Octree);
		break;
	case GUI_ID_QUIT: // File -> Quit
		Device->closeDevice();
		break;
	case GUI_ID_SKY_BOX_VISIBLE: // View -> Skybox
		menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
		SkyBox->setVisible(!SkyBox->isVisible());
		break;
	case GUI_ID_DEBUG_OFF: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem() + 1, false);
		menu->setItemChecked(menu->getSelectedItem() + 2, false);
		menu->setItemChecked(menu->getSelectedItem() + 3, false);
		menu->setItemChecked(menu->getSelectedItem() + 4, false);
		menu->setItemChecked(menu->getSelectedItem() + 5, false);
		menu->setItemChecked(menu->getSelectedItem() + 6, false);
		if (Model)
			Model->setDebugDataVisible(scene::EDS_OFF);
		break;
	case GUI_ID_DEBUG_BOUNDING_BOX: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
		if (Model)
			Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ scene::EDS_BBOX));
		break;
	case GUI_ID_DEBUG_NORMALS: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
		if (Model)
			Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ scene::EDS_NORMALS));
		break;
	case GUI_ID_DEBUG_SKELETON: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
		if (Model)
			Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ scene::EDS_SKELETON));
		break;
	case GUI_ID_DEBUG_WIRE_OVERLAY: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
		if (Model)
			Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ scene::EDS_MESH_WIRE_OVERLAY));
		break;
	case GUI_ID_DEBUG_HALF_TRANSPARENT: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
		if (Model)
			Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ scene::EDS_HALF_TRANSPARENCY));
		break;
	case GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem(), !menu->isItemChecked(menu->getSelectedItem()));
		if (Model)
			Model->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(Model->isDebugDataVisible() ^ scene::EDS_BBOX_BUFFERS));
		break;
	case GUI_ID_DEBUG_ALL: // View -> Debug Information
		menu->setItemChecked(menu->getSelectedItem() - 1, true);
		menu->setItemChecked(menu->getSelectedItem() - 2, true);
		menu->setItemChecked(menu->getSelectedItem() - 3, true);
		menu->setItemChecked(menu->getSelectedItem() - 4, true);
		menu->setItemChecked(menu->getSelectedItem() - 5, true);
		menu->setItemChecked(menu->getSelectedItem() - 6, true);
		if (Model)
			Model->setDebugDataVisible(scene::EDS_FULL);
		break;
	case GUI_ID_ABOUT: // Help->About
		guiMgr->showAboutText(Device, Caption, MessageText);
		break;
	case GUI_ID_MODEL_MATERIAL_SOLID: // View -> Material -> Solid
		if (Model)
			Model->setMaterialType(video::EMT_SOLID);
		break;
	case GUI_ID_MODEL_MATERIAL_TRANSPARENT: // View -> Material -> Transparent
		if (Model)
			Model->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		break;
	case GUI_ID_MODEL_MATERIAL_REFLECTION: // View -> Material -> Reflection
		if (Model)
			Model->setMaterialType(video::EMT_SPHERE_MAP);
		break;

	case GUI_ID_CAMERA_MAYA:
		cameraMgr->setActiveCamera(Device, Camera[0]);
		break;
	case GUI_ID_CAMERA_FIRST_PERSON:
		cameraMgr->setActiveCamera(Device, Camera[1]);
		break;
	}
}


/// <summary>
/// Handle key-up events.
/// </summary>
/// <param name="keyCode">The key code of the released key.</param>
/// <returns></returns>
bool TutEventReceiver::OnKeyUp(irr::EKEY_CODE keyCode)
{
	// Don't handle keys if we have a modal dialog open as it would lead 
	// to unexpected application behaviour for the user.
	if (hasModalDialog())
		return false;

	if (keyCode == irr::KEY_ESCAPE)
	{
		if (Device)
		{
			scene::ICameraSceneNode * camera =
				Device->getSceneManager()->getActiveCamera();
			if (camera)
			{
				camera->setInputReceiverEnabled(!camera->isInputReceiverEnabled());
			}
			return true;
		}
	}
	else if (keyCode == irr::KEY_F1)
	{
		if (Device)
		{
			IGUIElement* elem = Device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_POSITION_TEXT);
			if (elem)
				elem->setVisible(!elem->isVisible());
		}
	}
	else if (keyCode == irr::KEY_KEY_M)
	{
		if (Device)
			Device->minimizeWindow();
	}
	else if (keyCode == irr::KEY_KEY_L)
	{
		UseLight = !UseLight;
		if (Model)
		{
			Model->setMaterialFlag(video::EMF_LIGHTING, UseLight);
			Model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, UseLight);
		}
	}
	return false;
}


/// <summary>
/// Handle the event that one of the texture-filters was selected in the corresponding combobox.
/// </summary>
/// <param name="combo">The texture-filter combobox.</param>
void TutEventReceiver::OnTextureFilterSelected(IGUIComboBox* combo)
{
	s32 pos = combo->getSelected();
	switch (pos)
	{
	case 0:
		if (Model)
		{
			Model->setMaterialFlag(video::EMF_BILINEAR_FILTER, false);
			Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER, false);
			Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, false);
		}
		break;
	case 1:
		if (Model)
		{
			Model->setMaterialFlag(video::EMF_BILINEAR_FILTER, true);
			Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER, false);
		}
		break;
	case 2:
		if (Model)
		{
			Model->setMaterialFlag(video::EMF_BILINEAR_FILTER, false);
			Model->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
		}
		break;
	case 3:
		if (Model)
		{
			Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
		}
		break;
	case 4:
		if (Model)
		{
			Model->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, false);
		}
		break;
	}
}


/// <summary>
/// Dispatches an event to its corresponding handler.
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool TutEventReceiver::OnEvent(const SEvent& event)
{
	// Escape swaps Camera Input
	if (event.EventType == EET_KEY_INPUT_EVENT &&
		event.KeyInput.PressedDown == false)
	{
		if (OnKeyUp(event.KeyInput.Key))
			return true;
	}

	if (event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();
		IGUIEnvironment* env = Device->getGUIEnvironment();

		switch (event.GUIEvent.EventType)
		{
		case EGET_MENU_ITEM_SELECTED:
			// a menu item was clicked
			OnMenuItemSelected((IGUIContextMenu*)event.GUIEvent.Caller);
			break;

		case EGET_FILE_SELECTED:
		{
			// load the model file, selected in the file open dialog
			IGUIFileOpenDialog* dialog =
				(IGUIFileOpenDialog*)event.GUIEvent.Caller;
			//sceneMgr->loadModel(core::stringc(dialog->getFileName()).c_str());
			Device = Device;
			//sceneMgr->_Caption = Caption;
			sceneMgr->loadModel(core::stringc(dialog->getFileName()).c_str());
			// we need to uncheck the menu entries. would be cool to fake a menu event, but
			// that's not so simple. so we do it brute force
			gui::IGUIContextMenu* menuToUncheck = (gui::IGUIContextMenu*)Device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_TOGGLE_DEBUG_INFO, true);
			if (menuToUncheck)
				for (int item = 1; item < 6; ++item)
					menuToUncheck->setItemChecked(item, false);
			guiMgr->updateScaleInfo(Device, Model);
		}
		break;

		case EGET_SCROLL_BAR_CHANGED:

			// control skin transparency
			if (id == GUI_ID_SKIN_TRANSPARENCY)
			{
				const s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
				guiMgr->setSkinTransparency(pos, env->getSkin());
			}
			// control animation speed
			else if (id == GUI_ID_SKIN_ANIMATION_FPS)
			{
				const s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
				if (scene::ESNT_ANIMATED_MESH == Model->getType())
					((scene::IAnimatedMeshSceneNode*)Model)->setAnimationSpeed((f32)pos);
			}
			break;

		case EGET_COMBO_BOX_CHANGED:

			// control anti-aliasing/filtering
			if (id == GUI_ID_TEXTUREFILTER)
			{
				OnTextureFilterSelected((IGUIComboBox*)event.GUIEvent.Caller);
			}
			break;

		case EGET_BUTTON_CLICKED:

			switch (id)
			{
			case GUI_ID_BUTTON_SET_SCALE:
			{
				// set scale
				gui::IGUIElement* root = env->getRootGUIElement();
				core::vector3df scale;
				core::stringc s;

				s = root->getElementFromId(GUI_ID_X_SCALE, true)->getText();
				scale.X = (f32)atof(s.c_str());
				s = root->getElementFromId(GUI_ID_Y_SCALE, true)->getText();
				scale.Y = (f32)atof(s.c_str());
				s = root->getElementFromId(GUI_ID_Z_SCALE, true)->getText();
				scale.Z = (f32)atof(s.c_str());

				if (Model)
					Model->setScale(scale);
				guiMgr->updateScaleInfo(Device, Model);
			}
			break;
			case GUI_ID_BUTTON_SCALE_MUL10:
				if (Model)
					Model->setScale(Model->getScale()*10.f);
				guiMgr->updateScaleInfo(Device, Model);
				break;
			case GUI_ID_BUTTON_SCALE_DIV10:
				if (Model)
					Model->setScale(Model->getScale()*0.1f);
				guiMgr->updateScaleInfo(Device, Model);
				break;
			case GUI_ID_BUTTON_OPEN_MODEL:
				env->addFileOpenDialog(L"Please select a model file to open");
				break;
			case GUI_ID_BUTTON_SHOW_ABOUT:
				guiMgr->showAboutText(Device, Caption, MessageText);
				break;
			case GUI_ID_BUTTON_SHOW_TOOLBOX:
				guiMgr->createToolBox(Device, Model);
				break;
			case GUI_ID_BUTTON_SELECT_ARCHIVE:
				env->addFileOpenDialog(L"Please select your game archive/directory");
				break;
			}

			break;
		default:
			break;
		}
	}

	return false;
}



