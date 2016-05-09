#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	if (driverType == video::EDT_COUNT)
		return 1;

	// create device

	IrrlichtDevice *device = createDevice(driverType,
		core::dimension2d<u32>(512, 384));

	if (device == 0)
		return 1; // could not create selected driver.

	device->setWindowCaption(L"Irrlicht Engine - 2D Graphics Demo");

	video::IVideoDriver* driver = device->getVideoDriver();

	video::ITexture* images = driver->getTexture("../../irrmedia/2ddemo.png");

	driver->makeColorKeyTexture(images, core::position2d<s32>(0, 0)); //creates alpha channel texture based on top left color. It can also be created by specifying the color

	//load default font and another font. Create rectangles which specify the position of the two dragon-like things (imps) within the texture
	gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	gui::IGUIFont* font2 =
		device->getGUIEnvironment()->getFont("../../irrmedia/fonthaettenschweiler.bmp");

	core::rect<s32> imp1(349, 15, 385, 78);
	core::rect<s32> imp2(387, 15, 423, 78);

	//texture smoothing:
	driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

	while (device->run() && driver)
	{
		if (device->isWindowActive())
		{
			u32 time = device->getTimer()->getTime();

			driver->beginScene(true, true, video::SColor(255, 120, 102, 136));

			// draw fire & dragons background world
			driver->draw2DImage(images, core::position2d<s32>(50, 50),
				core::rect<s32>(0, 0, 342, 224), 0,
				video::SColor(255, 255, 255, 255), true);

			// draw flying imp
			driver->draw2DImage(images, core::position2d<s32>(164, 125),
				(time / 500 % 2) ? imp1 : imp2, 0,
				video::SColor(255, 255, 255, 255), true);

			// draw second flying imp with colorcylce
			//TO DO: try out colorcycle
			driver->draw2DImage(images, core::position2d<s32>(270, 105),
				(time / 500 % 2) ? imp1 : imp2, 0,
				video::SColor(255, (time) % 255, 255, 255), true);

			// draw some text
			if (font)
				font->draw(L"This demo shows that Irrlicht is also capable of drawing 2D graphics.",
				core::rect<s32>(130, 10, 300, 50),
				video::SColor(255, 255, 255, 255));

			// draw some other text
			if (font2)
				font2->draw(L"Also mixing with 3d graphics is possible.",
				core::rect<s32>(130, 20, 300, 60),
				video::SColor(255, time % 255, time % 255, 255));



			//Next, we draw the Irrlicht Engine logo (without using a color or an alpha channel). Since we slightly scale the image we use the prepared filter mode.
			driver->enableMaterial2D();
			driver->draw2DImage(images, core::rect<s32>(10, 10, 108, 48),
				core::rect<s32>(354, 87, 442, 118));
			driver->enableMaterial2D(false);

			//Finally draw a half-transparent rect under the mouse cursor.
			
			core::position2d<s32> m = device->getCursorControl()->getPosition();
			driver->draw2DRectangle(video::SColor(100, 255, 255, 255),
				core::rect<s32>(m.X - 20, m.Y - 20, m.X + 20, m.Y + 20));

			driver->endScene();
		}
	}

	device->drop();

	return 0;
}