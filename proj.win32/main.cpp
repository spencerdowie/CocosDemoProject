#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include "DisplayHandler.h"

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	//Create a console so we can use cout's for debugging. Cocos2D automatically hides this so you don't see it when playing your game.
	//*** What happens if you don't have this line of code? Try it to find out! *** //
	DISPLAY->createDebugConsole();

    //Create the application instance
	//The app delegate is essentially the base of your game
	//Simply leave these two lines of code here and everything should work fine
    AppDelegate app;
    return Application::getInstance()->run();
}
