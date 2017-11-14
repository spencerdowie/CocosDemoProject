#include "AppDelegate.h"
#include "DemoScene.h"

//Wrapper Classes
#include "InputHandler.h"
#include "DisplayHandler.h"

USING_NS_CC;

//--- Constructors and Destructors ---//
AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate()
{
}

//--- Virtual Methods ---//
bool AppDelegate::applicationDidFinishLaunching()
{
	//Create the window
	//The resolution of our window is 640x480 pixels
	//The title of the window is "Demo Scene for Cocos2D". This shows up on the toolbar at the top of the window
	//We do not want it fullscreen right now. If we did, our resolution parameters would be overwritten
	//The 2.0x zoom factor simply scales up our window so it is easier to see and work with. The window itself is 2x the size as well as everything being drawn inside it
	DISPLAY->init(640, 480, "Demo Scene for Cocos2D", false, 2.0f);

	//Create our main scene and tell the director to use it
	//The director is Cocos2D's game management system. It controls the scene switching, creating, etc. It is a singleton so there is only one instance of the class and it can be used everywhere
	//We are creating a new version of our demo scene and then telling the director to start using it
	Director* director = Director::getInstance();
	Scene* scene = DemoScene::createScene();
	director->runWithScene(scene);
	
	//Set up the input handler
	//This is another singleton so you can't make more than one instance of this class
	//INPUTS is actually a macro. It represents InputHandler::getInstance() which is exactly the same as the Director::getInstance() function we used above
	//This is a simple input handler to prevent having to handle the Cocos2D events yourself
	INPUTS->init();

	//Indicate everything succeeded with the launch
	return true;
}

void AppDelegate::applicationDidEnterBackground() 
{
	//This function is the opposite of applicationWillEnterForeground()
	//This function is called when your game / application is no longer the primary target on the user's screen
	//On mobile phones, this would be when the app is still running but the user has switched to a different app (ex: they answer a phone call)
	//On Windows, this is called when the user minimizes the game window
	//Some practical usage of this would to be pause the game until they come back. You can call director->pause() here to do just that.
	std::cout << "Entering Background..." << std::endl;
}

void AppDelegate::applicationWillEnterForeground() 
{
	//This function is the opposite of applicationDidEnterBackground()
	//This function is called when your game / application is becoming the primary target on the user's screen once again
	//On mobile phones, this would be when the app is still running and the user has switched back to it (ex: they finished with their phone call)
	//On Windows, this is called when the user selects the window again after minimizing it
	//This would be a good place to unpause the game if you had paused it or to perform other setup stuff like getting the music ready again
	std::cout << "Entering Foreground..." << std::endl;
}

