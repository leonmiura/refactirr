#ifndef __TUT_GUI_MANAGER
#define __TUT_GUI_MANAGER
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace gui;
using namespace video;
using namespace scene;

class TutGuiManager{
	public: 
		//public methods
		IGUIContextMenu* addMenu(IGUIEnvironment* env);
		IGUIToolBar* addToolBar(IGUIEnvironment* env);
		IGUIComboBox* addTextureFilterComboBoxToToolbar(IGUIEnvironment* env, IGUIToolBar* bar);
		void removeSkinTransparency(IGUIEnvironment* env);
		void createToolBox(IrrlichtDevice* device, ISceneNode* model);
		void updateToolBox(IrrlichtDevice* device, ISceneNode* model);
		void updateScaleInfo(IrrlichtDevice* device, ISceneNode* model);
		void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin);
		IGUIImage * addIrrlichtLogo(IGUIEnvironment* env);
		void setFont(IGUIEnvironment* env, stringc fontName);
		void showAboutText(IrrlichtDevice* device, core::stringw caption, core::stringw messageText);
};

#endif