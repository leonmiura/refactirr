#include <irrlicht.h>
//#include "driverChoice.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

//Declare a structure to hold some context for the event receiver so that it has it available inside its OnEvent() method
struct SAppContext	//why isn't this a class in the gui namespace?
{
	IrrlichtDevice* device;
	s32 counter;
	IGUIListBox* listbox;
	IGUIFileOpenDialog* filedialog;
};

enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR
};

//Set the skin transparency by changing the alpha values of all skin colors
void setSkinTransparency(s32 alpha, IGUISkin* skin)
{
	for (s32 i = 0; i < gui::EGDC_COUNT; i++) // EGDC_COUNT: count of all default skin colors
	{
		video::SColor col = skin->getColor((gui::EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((gui::EGUI_DEFAULT_COLOR)i,col);
	}
}


/*
The Event Receiver is not only capable of getting keyboard and mouse input events, but also events of the graphical user interface (gui). 
There are events for almost everything: Button click, Listbox selection change, events that say that a element was hovered and so on. 
To be able to react to some of these events, we create an event receiver. We only react to gui events, and if it's such an event, 
we get the id of the caller (the gui element which caused the event) and get the pointer to the gui environment.
*/
class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext& context) : Context(context){} //constructor with member initializer list. See: http://en.cppreference.com/w/cpp/language/initializer_list
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_GUI_EVENT) //we only respond to GUI events
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch (event.GUIEvent.EventType)
			{
				case gui::EGET_SCROLL_BAR_CHANGED: //if the event is a scrollbar value change, and the scrollbar is the transparency scrollbar, then we change the alpha value of the skin
					if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
					{
						s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
						setSkinTransparency(pos, env->getSkin());
					}
					break;
				case gui::EGET_BUTTON_CLICKED:
					switch(id)	
					{
						case GUI_ID_QUIT_BUTTON:
							Context.device->closeDevice();
							return true; //true because the event was handled, apparently
						case GUI_ID_NEW_WINDOW_BUTTON:
							{
								Context.listbox->addItem(L"Window created");
								Context.counter += 30;
								if (Context.counter > 200)
									Context.counter = 0;

								IGUIWindow* window = env->addWindow(rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
									false, //modal?
									L"Test window");

								env->addStaticText(L"Please close me",rect<s32>(35,35,140,150),
									true, //border?
									false, //wordwrap?
									window);
							}
							return true;
						case GUI_ID_FILE_OPEN_BUTTON:
							Context.listbox->addItem(L"File open");
							// There are some options for the file open dialog
							// We set the title, make it a modal window, and make sure 
							// that the working directory is restored after the dialog
							// is finished
							Context.filedialog = env->addFileOpenDialog(L"Please choose a file.",true,0,-1,true);
							return true;

						default:
							return false; //because no event was handled
					}
					break;
				case gui::EGET_FILE_SELECTED:
					Context.listbox->addItem(Context.filedialog->getFileName());
					break;
			}
		}

		return false;
	}


private:
	SAppContext& Context;
};


//Define some values that we'll use to identify individual GUI controls
int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType = E_DRIVER_TYPE::EDT_OPENGL;
	if (driverType == video::EDT_COUNT)
		return 1;

	// create device and exit if creation failed

	IrrlichtDevice * device = createDevice(driverType, core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1; // could not create selected driver.



	//Device creation was successful. Now we set the event receiver and store ponters to the driver and to the gui environment
	device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
	device->setResizable(true);

	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* env = device->getGUIEnvironment();

	//To make the font a bit nicer, we load an external font and set it as the new default fontin the skin --> I was looking for this
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("../../irrmedia/fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP); //maintain tooltip's default font

	//We add three buttons. The first one closes the engine. The second creates a window and the third opens a file open dialog. The third parameter is the id of the button, with which we can easily identify the button in the event receiver.
	env->addButton(rect<s32>(10, 240, 110, 240 + 32), 0, GUI_ID_QUIT_BUTTON,
		L"Quit", L"Exits Program");
	env->addButton(rect<s32>(10, 280, 110, 280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
		L"New Window", L"Launches a new Window");
	env->addButton(rect<s32>(10, 320, 110, 320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
		L"File Open", L"Opens a file");

	//Now, we add a static text and a scrollbar, which modifies the transparency of all gui elements. We set the maximum value of the scrollbar to 255, because that's the maximal value for a color value. Then we create an other static text and a list box.
	env->addStaticText(L"Transparent Control:", rect<s32>(150, 20, 350, 40), true);

	IGUIScrollBar* scrollbar = env->addScrollBar(true,
		rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);
	scrollbar->setPos(255); //becomes useless two lines later, doesn't it?
	setSkinTransparency(scrollbar->getPos(), env->getSkin()); //initial value (max)

	// set scrollbar position to alpha value of an arbitrary element
	scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha()); //will this not be the max by co. jones?

	env->addStaticText(L"Logging ListBox:", rect<s32>(50, 110, 250, 130), true);
	IGUIListBox * listbox = env->addListBox(rect<s32>(50, 140, 250, 210));
	env->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);

	env->addImage(driver->getTexture("../../irrmedia/irrlichtlogo2.png"),
		position2d<int>(10, 10));

	//That's all, we only have to draw everything.
	while (device->run() && driver)
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(0, 200, 200, 200));

			env->drawAll();

			driver->endScene();
		}

	device->drop();

	return 0;
}