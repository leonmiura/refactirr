#ifndef __TUT_EVENT_RECEIVER
#define __TUT_EVENT_RECEIVER

#include <irrlicht.h>
#include "TutCameraManager.h"
#include "TutGuiManager.h"
#include "TutGuiConstants.h"
#include "TutSceneManager.h"

using namespace irr;

/// <summary>
/// To get all the events sent by the GUI Elements, we need to create an event receiver.
/// If an event occurs, it checks the id of the caller and the event type, and starts an action based on these values. 
/// For example, if a menu item with id GUI_ID_OPEN_MODEL was selected, it opens a file-open-dialog.
/// </summary>
/// <seealso cref="IEventReceiver" />
class TutEventReceiver : public IEventReceiver
{
public:
	bool hasModalDialog();
	bool OnKeyUp(irr::EKEY_CODE keyCode);
	void OnKillFocus();
	void OnMenuItemSelected(IGUIContextMenu* menu);
	void OnTextureFilterSelected(IGUIComboBox* combo);
	bool OnEvent(const SEvent& event);
};

#endif

